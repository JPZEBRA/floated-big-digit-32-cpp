/* FLOATED BIG DIGIT CLASS */
/* CREATE  2021.02.06      */
/* REVISED 2021.05.24      */
/* Ver 0.9.5               */
/* Original by K-ARAI      */

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <float.h>

#include "FloatedBigDigit32.h"

/****************************************************************************/

int floatedBigDigit_K     = 5;
int floatedBigDigit_unit  = 100000;
int floatedBigDigit_order = 90000;

int floatedBigDigit_F     = 2;
int floatedBigDigit_sub   = 1000;

int floatedBigDigit_LMT   = 12000;
int floatedBigDigit_LM2   = 30000;
int floatedBigDigit_LMX   = 99990;

int floatedBigDigit_LST   = 5;
int floatedBigDigit_LSX   = 7;

/****************************************************************************/

int floatedBigDigit_keta = 100;

/****************************************************************************/

bool floatedBigDigitCashedE = false;
FloatedBigDigit32* floatedBigDigitCashE;
int floatedBigDigitCashedE_R;

bool floatedBigDigitCashedPI = false;
FloatedBigDigit32* floatedBigDigitCashPI;
int floatedBigDigitCashedPI_R;


bool floatedBigDigitCashedLB = false;
FloatedBigDigit32* floatedBigDigitCashLB;
int floatedBigDigitCashedLB_L = 0;

/****************************************************************************/
/****************************************************************************/

int Fbd32SetRandomSeed(int seed) {

    srand(seed);

    return floatedBigDigitOK;
}

/****************************************************************************/

int Fbd32LimitBreak() {

    floatedBigDigit_LM2 = floatedBigDigit_LMX;
    floatedBigDigit_LST = floatedBigDigit_LSX;

    return floatedBigDigitOK;
}

/****************************************************************************/

int SetFloatedBigDigit32Keta(int k) {

    if(k<=0) return floatedBigDigitERR;

    floatedBigDigit_keta = k;

    FreeFloatedBigDigit32();

    return floatedBigDigitOK;

}

/****************************************************************************/

int FreeFloatedBigDigit32() {


    if(floatedBigDigitCashedE ) delete floatedBigDigitCashE;

    floatedBigDigitCashedE = false;

    if(floatedBigDigitCashedPI) delete floatedBigDigitCashPI;

    floatedBigDigitCashedPI = false;

    if(floatedBigDigitCashedLB) delete floatedBigDigitCashLB;

    floatedBigDigitCashedLB = false;

    return floatedBigDigitOK;

}


/****************************************************************************/

FloatedBigDigit32::FloatedBigDigit32() {

    this->N = ((floatedBigDigit_keta - 1)/ floatedBigDigit_K + 1);

    this->Val = (int*)malloc(sizeof(int)*(this->N + floatedBigDigit_F + 1));

    this->shiftPoint = 0;

    this->small = false;

    this->minus = false;
}

/****************************************************************************/

FloatedBigDigit32::~FloatedBigDigit32() {

    free(this->Val);

}

/****************************************************************************/


int FloatedBigDigit32::clear() {

    for(int i=0;i<=this->N+floatedBigDigit_F;i++) this->Val[i] = 0;

    this->shiftPoint = 0;

    this->small = false;

    this->minus = false;

    return floatedBigDigitOK;

}   

/****************************************************************************/

int FloatedBigDigit32::set(int val) {

    this->clear();

    if(val<0) {
        this->minus = true;
        val = -val;
    }

    if(val > floatedBigDigit_unit) {
        this->overflow();
        return floatedBigDigitERR;
    }

    if(val == floatedBigDigit_unit) {
        this->Val[0] = 1;
        this->shift(+1);        
    } else {
        this->Val[0] = val;
    }

    return floatedBigDigitOK;

}   

/****************************************************************************/

int FloatedBigDigit32::Set(const char* str) {

    bool readError = false;
    bool valMinus = false;
    int idx = 0;
    char rdc;

    this->clear();

    //STEP 0
    rdc = str[idx];
    if(rdc=='+') { ++idx; valMinus = false; }
    if(rdc=='-') { ++idx; valMinus = true; }

    //STEP 1
    do {
        rdc = str[idx++];
        if(rdc=='\0' || rdc=='.' || rdc=='e' || rdc == 'E' ) break;
        if(rdc>='0' && rdc<='9') {
            this->mul(10);
            this->add(rdc-'0');
        } else {
            readError = true;
        }
    } while(!readError);

    //STEP 2
    if(rdc=='.') {
        int ord = 0;
        do {
            rdc = str[idx++];
            if(rdc=='\0' || rdc=='e' || rdc == 'E' ) break;
            if(rdc>='0' && rdc<='9') {
                this->mul(10);
                this->add(rdc-'0');
                ++ord;
            } else {
                readError = true;
            }
         } while(!readError);

         for(int i=0;i<ord;i++) this->div(10);
    }

    //STEP 3
    if(rdc=='e' || rdc == 'E' ) {

        bool odm = false;
        int odn = 0;

        rdc = str[idx];
        if(rdc=='+') { ++idx; odm = false; }
        if(rdc=='-') { ++idx; odm = true; }

        do {
            rdc = str[idx++];
            if(rdc=='\0') break;
            if(rdc>='0' && rdc<='9') {
                odn *= 10;
                odn += ( rdc-'0');
                if( odn >= floatedBigDigit_order ) break;
            } else {
                readError = true;
            }
        } while(!readError);

        if( odn >= floatedBigDigit_order ) {
            if(!odm) this->overflow();
            if( odm) this->set(0);
        } else {
           if(!odm) for(int i=0;i<odn;i++) this->mul(10);
           if( odm) for(int i=0;i<odn;i++) this->div(10);
        }

    }

    this->minus = valMinus;

    if(readError) {
        this->overflow();
        return floatedBigDigitERR;
    }

    if(this->checkOver()) return floatedBigDigitERR;

    return floatedBigDigitOK;

}

/****************************************************************************/

bool FloatedBigDigit32::lastBit() {

    int last_bit_boost = floatedBigDigit_LST;

    if(this->isOver()) return true;

    if(this->isEmpty()) return true;

    if(!this->isSmall()) return false;

    bool rf = this->minus;
    this->minus = false;

    if( floatedBigDigitCashedLB && floatedBigDigitCashLB->N != this->N ) {
        delete floatedBigDigitCashLB;
        floatedBigDigitCashedLB = false;
    }

    if( floatedBigDigitCashedLB && floatedBigDigitCashedLB_L != last_bit_boost ) {
        delete floatedBigDigitCashLB;
        floatedBigDigitCashedLB = false;
    }

    if(!floatedBigDigitCashedLB ) {
        floatedBigDigitCashLB  = new FloatedBigDigit32();
        floatedBigDigitCashedLB = true;
        floatedBigDigitCashedLB_L = last_bit_boost;
        floatedBigDigitCashLB->set(1);
        floatedBigDigitCashLB->shiftPoint = - ( floatedBigDigitCashLB->N + floatedBigDigit_F*last_bit_boost );
    }

    bool ret = !(this->Compare(floatedBigDigitCashLB)>=0);

    this->minus = rf;

    return ret;

}

/****************************************************************************/

int FloatedBigDigit32::Copy(FloatedBigDigit32* V) {

    for(int i=0;i<=this->N+floatedBigDigit_F;i++) this->Val[i] = ( i<=V->N+floatedBigDigit_F ) ? V->Val[i] : 0;

    this->shiftPoint = V->shiftPoint;

    this->small = V->small;
    this->minus = V->minus;

    return floatedBigDigitOK;

}   

/****************************************************************************/

int FloatedBigDigit32::size() {

    return this->N;

}

/****************************************************************************/

int FloatedBigDigit32::zero_pos() {

    if(this->isOver()) return 0;

    return this->shiftPoint;

}

/****************************************************************************/

int FloatedBigDigit32::order() {

    if(this->isOver()) return 0;

    return this->shiftPoint * floatedBigDigit_K;

}

/****************************************************************************/

int FloatedBigDigit32::valid_size() {

    if(this->isOver()) return 0;

    this->shiftMax();

    int k;

    for(k=this->N+1;k>=1;k--) if(this->Val[k-1]!=0) break;

    return k;

}

/****************************************************************************/

int FloatedBigDigit32::digit(int idx) {

    if(this->isOver()) return 0;

    if(idx>=0&&idx<=this->N+floatedBigDigit_F) return this->Val[idx];

    return 0;

}

/****************************************************************************/

int FloatedBigDigit32::value(int idx) {

    if(this->isOver()) return 0;

    if(this->isBig()) {
        return this->digit( this->shiftPoint - idx );
    } else {
        return this->digit( idx + this->shiftPoint );
    }

}

/****************************************************************************/

bool FloatedBigDigit32::checkOver() {

    if( this->shiftPoint >=  floatedBigDigit_order / floatedBigDigit_K ) this->overflow();
  
    if( this->shiftPoint <  -floatedBigDigit_order / floatedBigDigit_K ) this->set(0);

    if( this->Val[0] >= floatedBigDigit_unit ) this->overflow();

    return this->isOver();

}

/****************************************************************************/

bool FloatedBigDigit32::overflow() {

    this->set(0);

    this->shiftPoint = floatedBigDigit_unit;

    return true;

}

/****************************************************************************/

bool FloatedBigDigit32::isMinus() {

    if(this->isOver()) {
        return false;
    }


    return this->minus;

}

/****************************************************************************/

bool FloatedBigDigit32::isSmall() {

    if(this->isOver()) {
        return false;
    }

    this->shiftMax();

    this->small = true;

    if(this->shiftPoint>=0) {
        for(int i = this->shiftPoint+1;i<=this->N+floatedBigDigit_F;i++) if(this->Val[i]!=0) return true;
        this->small = false;
        return false;
    }

    return true;

}

/****************************************************************************/

bool FloatedBigDigit32::isSeed() {

    if(this->isOver()) {
        return false;
    }

    if(this->shiftPoint!=0) return false;

    return !this->isSmall();

}

/****************************************************************************/

bool FloatedBigDigit32::isBig() {

    if(this->isOver()) {
        return false;
    }

    if(this->shiftPoint<=0) return false;

    return true;

}

/****************************************************************************/

bool FloatedBigDigit32::isExp() {

    if(this->isOver()) {
        return false;
    }

    if(this->isZero()) return false;

    if(this->shiftPoint>=this->N) return true;

    if(this->valid_size()>this->shiftPoint+1) return true;

    return false;

}

/****************************************************************************/

bool FloatedBigDigit32::isOver() {

    if(this->shiftPoint == floatedBigDigit_unit) return true;

    return false;

}

/****************************************************************************/

bool FloatedBigDigit32::isZero() {

    if(this->isOver()) {
        return false;
    }

    for(int i=0;i<=this->N;i++) if(this->Val[i]!=0) return false;

    return true;

}

/****************************************************************************/

bool FloatedBigDigit32::isEmpty() {

    if(this->isOver()) {
        return false;
    }

    for(int i=0;i<=this->N+floatedBigDigit_F;i++) if(this->Val[i]!=0) return false;

    return true;

}

/****************************************************************************/

int FloatedBigDigit32::FD() {

    if(this->isOver()) {
        return floatedBigDigitERR;
    }

    if(!this->isSmall()) return floatedBigDigitOK;

    for(int i = this->N+1;i<=this->N+floatedBigDigit_F;i++) this->Val[i] = 0;

    return floatedBigDigitOK;

}

/****************************************************************************/

int FloatedBigDigit32::FR() {

    if(this->isOver()) {
        return floatedBigDigitERR;
    }

    if(!this->isSmall()) return floatedBigDigitOK;

    int CR = floatedBigDigit_unit/2;

    for(int i = this->N+1;i>=0;i--) {
        int v = this->Val[i] + CR;
        CR = (v/floatedBigDigit_unit);
        this->Val[i] = v % floatedBigDigit_unit;
    }

    if(CR>0) {

        FloatedBigDigit32* CC = new FloatedBigDigit32();

        CC->set(CR);
        CC->shiftPoint = this->shiftPoint  + 1;
        if(this->isMinus()) {
            this->Sub(CC);
        } else {
            this->Add(CC);
        }

        delete CC;
    }

    for(int i = this->N+1;i<=this->N+floatedBigDigit_F;i++) this->Val[i] = 0;

    return floatedBigDigitOK;

}

/****************************************************************************/

int FloatedBigDigit32::Fix() {

    if(this->isOver()) {
        return floatedBigDigitERR;
    }

    if(this->shiftPoint<0) {
        this->clear();
        return floatedBigDigitOK;
    }

    for(int i=this->shiftPoint+1; i<=this->N+floatedBigDigit_F ; i++) this->Val[i] = 0;

    this->small = false;

    return floatedBigDigitOK;

}

/****************************************************************************/

int FloatedBigDigit32::Round() {

    if(this->isOver()) {
        return floatedBigDigitERR;
    }

    if(this->shiftPoint<-1) {
        this->clear();
        return floatedBigDigitOK;
    }

    bool mf = this->minus;
    this->minus = false;

    FloatedBigDigit32* CC = new FloatedBigDigit32();

    CC->set(1);
    CC->div(2);
    this->Add(CC);

    delete CC;

    for(int i=this->shiftPoint+1; i<=this->N+floatedBigDigit_F ; i++) this->Val[i] = 0;

    this->small = false;
    this->minus = mf;

    return floatedBigDigitOK;

}

/****************************************************************************/

int FloatedBigDigit32::Int() {

    if(this->isOver()) {
        return floatedBigDigitERR;
    }

    if(this->shiftPoint<-1) {
        this->clear();
        return floatedBigDigitOK;
    }

    FloatedBigDigit32* CC = new FloatedBigDigit32();

    CC->set(1);
    CC->div(2);
    this->Sub(CC);

    delete CC;

    return this->Round();

}

/****************************************************************************/

int FloatedBigDigit32::Abs() {

    if(this->isOver()) {
        return floatedBigDigitERR;
    }

    this->minus = false;

    return floatedBigDigitOK;

}

/****************************************************************************/

int FloatedBigDigit32::Sig() {

    if(this->isOver()) {
        return floatedBigDigitERR;
    }

    this->minus = !this->minus;

    return floatedBigDigitOK;

}

/****************************************************************************/

int FloatedBigDigit32::shift(int val) {

    if(this->isOver()) {
        return floatedBigDigitERR;
    }

    return this->shiftSmall(val);

}

/****************************************************************************/

int FloatedBigDigit32::shiftSmall(int val) {

    if(this->isOver()) {
        return floatedBigDigitERR;
    }

    if(val==0) return floatedBigDigitOK;
    if(this->isEmpty()) return floatedBigDigitOK;

    if(val<0) {

        if(-val>this->N+floatedBigDigit_F) {
            this->clear();
            return floatedBigDigitOK;
        }

        int CR = (this->Val[this->N+floatedBigDigit_F+val+1]>floatedBigDigit_unit/2) ? 1: 0;

        for(int i=this->N+floatedBigDigit_F;i>=0;i--) this->Val[i] = ( (i+val>=0) ? this->Val[i+val] : 0);

        return floatedBigDigitOK;

    } else {

        if(val>this->N+floatedBigDigit_F) {
            this->overflow();
            return floatedBigDigitERR;
        }

        for(int i=0;i<val;i++) {
            if(this->Val[i]>0) {
                this->overflow();
                return floatedBigDigitERR;
            }
        }

        for(int i=0;i<=this->N+floatedBigDigit_F;i++) {
            int v = ((i+val<=this->N+floatedBigDigit_F) ? this->Val[i+val] : 0);
            this->Val[i] = v;
        }

    }

    return floatedBigDigitOK;

}

/****************************************************************************/

int FloatedBigDigit32::shiftMax() {

    if(this->isOver()) {
        return floatedBigDigitERR;
    }

    if(this->isEmpty()) {
        this->clear();
        return 0;
    }

    int sf;

    for(sf = 0 ; sf < this->N + floatedBigDigit_F; sf++) {
        if(this->Val[sf] !=0 ) break;
    }

    this->shift(sf);

    this->shiftPoint -= sf;

    return sf;

}

/****************************************************************************/
/****************************************************************************/

int FloatedBigDigit32::mul(int val) {

    return this->mulSmall(val);

}

/****************************************************************************/

int FloatedBigDigit32::mulSmall(int val) {

    if(this->isOver()) {
        return floatedBigDigitERR;
    }

    if(val==0) {
        this->clear();
        return floatedBigDigitOK;
    }

    if(abs(val) > floatedBigDigit_unit) {
        this->overflow();
        return floatedBigDigitERR;
    }

    bool rf = this->minus;
    this->minus = false;

    if(val<0) {
        rf = !rf;
        val = - val;
    }

    if(val==1) {
        this->minus = rf;
        return floatedBigDigitOK;
    }

    int CR = 0;
    for(int i = this->N+floatedBigDigit_F;i>=0;i--) {

        // low bit size multiple !

        int v = this->Val[i];
        int vh = v / floatedBigDigit_sub;
        int vl = v % floatedBigDigit_sub;

        vh *= val;
        vl *= val;

        vl += CR;

        vh += vl / floatedBigDigit_sub;
        vl = vl % floatedBigDigit_sub;

        CR = vh / ( floatedBigDigit_unit / floatedBigDigit_sub );
        vh = vh % ( floatedBigDigit_unit / floatedBigDigit_sub );

        this->Val[i] = vh*floatedBigDigit_sub + vl;

    }

    if(CR>0) {
        FloatedBigDigit32* BB = new FloatedBigDigit32();
        BB->set(CR);
        BB->shiftPoint = this->shiftPoint + 1;
        this->Add(BB);
        delete BB;
    }

    this->minus = rf;

    if(this->checkOver()) return floatedBigDigitERR;

    this->isSmall();

    return floatedBigDigitOK;

}   

/****************************************************************************/
/****************************************************************************/

int FloatedBigDigit32::div(int val,bool div_fast) {

    if(div_fast) return this->divSmall(val);

    FloatedBigDigit32* V = new FloatedBigDigit32();

    V->set(val);

    int ret = this->Div(V,div_fast);

    delete V;

    return ret;

}

/****************************************************************************/

int FloatedBigDigit32::divSmall(int val) {

    if(this->isOver()) {
        return floatedBigDigitERR;
    }

    if(val==0) {
        this->overflow();
        return floatedBigDigitERR;
    }

    if(abs(val) > floatedBigDigit_unit) {
        this->overflow();
        return floatedBigDigitERR;
    }

    if(val<0) {
        this->minus = !this->minus;
        val = - val;
    }

    if(val==1) {
        return floatedBigDigitOK;
    }

    int CR = 0;
    for(int i = 0;i<=this->N+floatedBigDigit_F;i++) {

        // low bit size division !

        int v = this->Val[i];
        int vh = v / floatedBigDigit_sub;
        int vl = v % floatedBigDigit_sub;

        vh += CR * ( floatedBigDigit_unit / floatedBigDigit_sub );

        int ch = vh % val;
        vh = vh / val;

        vl += ch * floatedBigDigit_sub;

        int cl = vl % val;
        vl = vl / val;

        this->Val[i] = vh*floatedBigDigit_sub + vl;

        CR = cl;

    }       

    CR = (CR*2/val>=1) ? 1: 0;

    if(this->checkOver()) return floatedBigDigitERR;

    this->isSmall();

    return floatedBigDigitOK;

}   

/****************************************************************************/
/****************************************************************************/

int FloatedBigDigit32::add(int val) {

    return this->addSmall(val);

}

/****************************************************************************/

int FloatedBigDigit32::addSmall(int val) {

    if(this->isOver()) {
        return floatedBigDigitERR;
    }

    if(val==0) {
        return floatedBigDigitOK;
    }

    if(abs(val) > floatedBigDigit_unit) {
        this->overflow();
        return floatedBigDigitERR;
    }

    if(val<0) {
        return this->subSmall(-val);
    }

    if(this->minus) {
        this->minus = !this->minus;
        int ret = this->subSmall(val);
        this->minus = !this->minus;
        return ret;
    }

    FloatedBigDigit32* BB = new FloatedBigDigit32();

    BB->set(val);

    bool ret = this->Add(BB);

    delete BB;

    if(this->checkOver()) return floatedBigDigitERR;

    this->isSmall();

    return ret;

}   

/****************************************************************************/
/****************************************************************************/

int FloatedBigDigit32::sub(int val) {


    return this->subSmall(val);

}

/****************************************************************************/

int FloatedBigDigit32::subSmall(int val) {

    if(this->isOver()) {
        return floatedBigDigitERR;
    }

    if(val==0) {
        return floatedBigDigitOK;
    }

    if(abs(val) > floatedBigDigit_unit) {
        this->overflow();
        return floatedBigDigitERR;
    }

    if(val<0) {
        return this->addSmall(-val);
    }

    if(this->minus) {
        this->minus = !this->minus;
        int ret = this->addSmall(val);
        this->minus = !this->minus;
        return ret;
    }

    FloatedBigDigit32* BB = new FloatedBigDigit32();

    BB->set(val);

    bool ret = this->Sub(BB);

    delete BB;

    if(this->checkOver()) return floatedBigDigitERR;

    this->isSmall();

    return ret;

}

/****************************************************************************/
/****************************************************************************/

int FloatedBigDigit32::compare(int val) {

    FloatedBigDigit32* CC = new FloatedBigDigit32();

    CC->set(val);

    int ret = this->Compare(CC);

    delete CC;

    return ret;

}

/****************************************************************************/

int FloatedBigDigit32::Compare(FloatedBigDigit32* V) {

    if(this->isOver()) {
        return 0;
    }

    if(V->isOver()) {
        return 0;
    }

    int GT = +1;
    int LT = -1;
    int EQ =  0;
    int RV =  1;

    if( this->isMinus() && !V->isMinus()) return LT;
    if(!this->isMinus() &&  V->isMinus()) return GT;
    if( this->isMinus() &&  V->isMinus()) RV = -1;

    if( this->isZero()){
        if( V->isZero() ) return EQ;
        if(!V->isMinus()) return LT;
        if( V->isMinus()) return GT;
    }

    int sf = V->shiftPoint - this->shiftPoint;

    int max1 = V->shiftPoint;
    int min1 = max1 - (V->N+floatedBigDigit_F);

    int max2 = this->shiftPoint;
    int min2 = max2 - (this->N+floatedBigDigit_F);

    int max = ( max1 >= max2 ) ? max1 : max2;
    int min = ( min1 <= min2 ) ? min1 : min2;

    int width = max - min;

    if( sf >= 0 ) {

        for(int i = 0;i<=width;i++) {

            if(this->digit(i-sf)>V->digit(i)) return GT*RV;
            if(this->digit(i-sf)<V->digit(i)) return LT*RV;

        }

    } else {

        for(int i = 0;i<=width;i++) {

            if(this->digit(i)>V->digit(i+sf)) return GT*RV;
            if(this->digit(i)<V->digit(i+sf)) return LT*RV;

         }

    }

    return EQ;

}

/****************************************************************************/
/****************************************************************************/

int FloatedBigDigit32::toString(char* str,int n,bool rawdata) {

    if(n>0) str[0] = '\0';

    if(this->isOver()) {
        return floatedBigDigitERR;
    }

    if(n<(this->N+1)*(floatedBigDigit_K+1)+8) return floatedBigDigitERR;

    int ptr = 0;
    int shift = 0;

    FloatedBigDigit32* AA = new FloatedBigDigit32();

    AA->Copy(this);

    // NORMALIZE
    if(!rawdata) {
        while(AA->Val[0]>=10) {
            AA->div(10);
            ++ shift;
        }
        AA->FR();
        if(AA->Val[0]>=10) AA->div(10);
    }

    if (AA->isMinus()) {
        str[ptr++] = '-';
    } else {
        str[ptr++] = '+';
    }

    for(int i=0;i<=this->N;i++) {
        int val = AA->Val[i];
        int p = floatedBigDigit_unit;
        for(int j=0;j<floatedBigDigit_K;j++) {
            if(AA->isOver()) {
                str[ptr++] = '*';
            } else if(val>=p) {
                str[ptr++] = 'P';
            } else if(val<0) {
                str[ptr++] = 'M';
            } else {
                p /= 10;
                int v = val / p;
                str[ptr++] = '0' + v;
                val -= v*p;
            }
        }
        if(i==0) {
            str[ptr++] = '.';
        } else {
            str[ptr++] = ' ';
        }
    }

    str[ptr++] = 'E';

    int ord = this->order();

    ord += shift;

    if(ord>=0) {
        str[ptr++] = '+';
    } else {
        str[ptr++] = '-';
        ord = (-ord);
    }

    int p = 100000;

    for(int j=0;j<5;j++) {
        if(AA->isOver()) {
            str[ptr++] = '*';
        } else if(ord>=p) {
            str[ptr++] = 'P';
        } else if(ord<0) {
            str[ptr++] = 'M';
        } else {
            p /= 10;
            int v = ord / p;
            str[ptr++] = '0' + v;
            ord -= v*p;
        }
    }

    str[ptr++] = '\0';

    delete AA;

    return floatedBigDigitOK;

}

/****************************************************************************/

int FloatedBigDigit32::footString(char* str,int n) {

    if(n>0) str[0] = '\0';

    if(this->isOver()) {
        return floatedBigDigitERR;
    }

    int ptr = 0;

    if(n<(floatedBigDigit_K+1)*floatedBigDigit_F+1) return floatedBigDigitERR;

    for(int i=this->N+1;i<=this->N+floatedBigDigit_F;i++) {
        int val = this->Val[i];
        int p = floatedBigDigit_unit;
        for(int j=0;j<floatedBigDigit_K;j++) {
            p /= 10;
            int v = val / p;
            str[ptr++] = '0' + v;
            val -= v*p;
        }
        str[ptr++] = ' ';
    }

    str[ptr++] = '\0';

    return floatedBigDigitOK;

}

/****************************************************************************/

int FloatedBigDigit32::toString2(char* str,int keta,int small) {

    if(keta<0) keta = 0;

    if(small<0) small = 0;

    if(keta>0) str[0] = '\0';

    if(this->isOver()) {
        return floatedBigDigitERR;
    }

    FloatedBigDigit32* AA = new FloatedBigDigit32();

    FloatedBigDigit32* BB = new FloatedBigDigit32();

    AA->Copy(this);

    BB->set(5);

    for(int i=0;i<=small;i++) BB->div(10);

    bool rf = AA->minus;

    AA->minus = false;

    AA->Add(BB);

    AA->minus = rf;

    delete BB;

    int ptr;

    ++keta;

    int len = keta;
    if (small>0) len += small + 1;

    memset(str,'0',len);
    str[len] = '\0';
    if (small>0) str[keta] = '.';

    if(AA->isMinus()) {
        str[0] = '-';
    } else {
        str[0] = '+';
    }

    ptr = keta - 1;

    if(AA->isOver()){
        memset(str,'*',len);
        if (small>0) str[keta] = '.';
        delete AA;
        return floatedBigDigitERR;
    }

    for(int i=AA->zero_pos();i>=0;i--) {

        int val = AA->digit(i);

        for(int j=0;j<5;j++) {
            if(ptr<1) {
                if(val>0) {
                    memset(str,'*',len);
                    if (small>0) str[keta] = '.';
                    delete AA;
                    return floatedBigDigitERR;
                }
            } else {
                char p = '0' + ( val % 10 );
                str[ptr--] = p;
                val /= 10;
            }
        }

    }

    for( int i=1;i<=(small-1)/5+1;i++) {

        int val = AA->digit(this->zero_pos()+i);

        ptr = keta + i*5;

        for(int j=0;j<5;j++) {
            if(ptr<len) {
              char p = '0' + ( val % 10 );
              str[ptr] = p;
             }
             ptr--;
             val /= 10;
        }

    }

    delete AA;

    return floatedBigDigitOK;

}

/****************************************************************************/

int FloatedBigDigit32::LoadString(const char* str) {

    int ptr = 0;

    char c;

    int val;
    int ord;

    bool vm = false;
    bool om = false;

    bool readError = false;

    this->set(0);

    c = str[ptr];

    if       (c=='+') {
        vm = false;
        ptr++;
    } else if(c=='-') {
        vm = true;
        ptr++;
    } else {
        readError = true;
    }

    while(!readError) {

        c = str[ptr];
        if( c == '\n') break;
        if( c == 'E') break;

        val = 0;

        for(int i=0;i<floatedBigDigit_K;i++) {
            c = str[ptr];
            if( c >= '0' && c <= '9') {
                val *= 10;
                val += ( c - '0' );
                ptr++;
            } else {
                readError = true;
                break;
            }
        }

        c = str[ptr];
        if( c == ' ' || c == '.' ) {
            ptr++;
        } else {
            readError = true;
        }

        if(!readError) {
            for(int i=0;i<5;i++) this->mul(10);
            this->add(val);
        }

    }

    if(!readError && c == 'E' ) {

        ptr++;
        c = str[ptr];

        if       (c=='+') {
            om = false;
            ptr++;
        } else if(c=='-') {
            om = true;
            ptr++;
        } else {
            readError = true;
        }

        ord = 0;

        for(int i=0;i<5;i++) {
            c = str[ptr];
            if( c >= '0' && c <= '9') {
                ord *= 10;
                ord += ( c - '0' );
                ptr++;
            } else {
                readError = true;
                break;
            }
        }

        if(!readError) {
            if(om) ord = - ord;
            int sf = ord / floatedBigDigit_K;
            ord = ord % floatedBigDigit_K;
            if(ord<0) {
              --sf;
              ord += floatedBigDigit_K;
            }
            for(int i=0;i<ord;i++) this->mul(10);
            this->shiftPoint = sf;
        }

    }

    if(readError) {
        this->overflow();
        return floatedBigDigitERR;
    }

    this->minus = vm;

    return floatedBigDigitOK;
            
}

/****************************************************************************/

double FloatedBigDigit32::toDouble() {

    double ret = 0.0;
    int width = 10;

    if(this->isOver()) {
        return DBL_MAX;
    }

    if(width>this->N) width = this->N; 

    for(int i=0;i<=width;i++) {
        ret *= floatedBigDigit_unit;
        ret += this->Val[i];
    }

    int ord = this->order();

    ord -= width*floatedBigDigit_K;

    if(ord>0) {
        for(int j=1;j<=ord;j++) ret *= 10;
    }

    if(ord<0) {
        for(int j=-1;j>=ord;j--) ret /= 10;
    }

    if(this->isMinus()) {
        ret = - ret;
    }

    return ret;

}

/****************************************************************************/
/****************************************************************************/

int FloatedBigDigit32::Add(FloatedBigDigit32* V) {

    if(this->isOver()) {
        return floatedBigDigitERR;
    }

    if(V->isEmpty()) {
        return floatedBigDigitOK;
    }

    if(this->isEmpty()) {
        this->Copy(V);
        return floatedBigDigitOK;
    }

    return this->AddSmall(V);

}

/****************************************************************************/

int FloatedBigDigit32::AddSmall(FloatedBigDigit32* V) {

    if(V->isMinus()) {
        V->minus = !V->minus;
        int ret = this->SubSmall(V);
        V->minus = !V->minus;
        return ret;
    }

    if(this->isMinus()) {
        this->minus = !this->minus;
        int ret = this->SubSmall(V);
        this->minus = !this->minus;
        return ret;
    }

    FloatedBigDigit32* AA = new FloatedBigDigit32();

    AA->clear();

    int idx1,idx2,idx3;

    for(idx1=0;idx1<this->N+floatedBigDigit_F;idx1++) if(this->Val[idx1]!=0) break;
    for(idx2=0;idx2<V->N+floatedBigDigit_F;idx2++) if(V->Val[idx2]!=0) break;

    idx3 = (idx1<=idx2) ? idx1 : idx2;

    int sf1 = this->shiftPoint;
    int sf2 = V->shiftPoint;
    int sf3 = (sf1>=sf2) ? sf1 : sf2;

    AA->shiftPoint = sf3;

    int CR = 0;

    for(int i=this->N+floatedBigDigit_F;i>=0;i--) {

        int pos1 = i + idx3 + sf1 - sf3;
        int pos2 = i + idx3 + sf2 - sf3;

        int val = CR;

        val += this->digit(pos1);
        val += V->digit(pos2);

        if ( val >= floatedBigDigit_unit) {
            CR = 1;
            val -= floatedBigDigit_unit;
        } else {
            CR = 0;
        }

        AA->Val[i] = val;

    }

    if(CR>0) {
        AA->shift(-1);
        AA->Val[0] = CR;
        AA->shiftPoint += 1;
    }

    this->Copy(AA);

    this->isSmall();

    delete AA;

    if(this->checkOver()) return floatedBigDigitERR;

    return floatedBigDigitOK;

}

/****************************************************************************/
/****************************************************************************/

int FloatedBigDigit32::Sub(FloatedBigDigit32* V) {

    if(this->isOver()) {
        return floatedBigDigitERR;
    }

    if(V->isEmpty()) {
        return floatedBigDigitOK;
    }

    if(this->isEmpty()) {
        this->Copy(V);
        this->minus = !this->minus;
        return floatedBigDigitOK;
    }

    return this->SubSmall(V);

}

/****************************************************************************/

int FloatedBigDigit32::SubSmall(FloatedBigDigit32* V) {

    if(V->isMinus()) {
        V->minus = !V->minus;
        int ret = this->AddSmall(V);
        V->minus = !V->minus;
        return ret;
    }

    if(this->isMinus()) {
        this->minus = !this->minus;
        int ret = this->AddSmall(V);
        this->minus = !this->minus;
        return ret;
    }

    if(this->Compare(V)<0) {
        FloatedBigDigit32* C = new FloatedBigDigit32();
        C->Copy(this);
        this->Copy(V);
        int ret = this->SubSmall(C);
        this->minus = true;
        this->isSmall();
        delete C;
        return ret;
    }

    FloatedBigDigit32* AA = new FloatedBigDigit32();


    AA->clear();

    int idx1,idx2,idx3;
    for(idx1=0;idx1<this->N+floatedBigDigit_F;idx1++) if(this->Val[idx1]!=0) break;
    for(idx2=0;idx2<V->N+floatedBigDigit_F;idx2++) if(V->Val[idx2]!=0) break;

    idx3 = (idx1<=idx2) ? idx1 : idx2;

    int sf1 = this->shiftPoint;
    int sf2 = V->shiftPoint;

    int sf3 = (sf1>=sf2) ? sf1 : sf2;

    AA->shiftPoint = sf3;

    int CR = 0;

    for(int i=this->N+floatedBigDigit_F;i>=0;i--) {

        int pos1 = i + idx3 + sf1 - sf3;
        int pos2 = i + idx3 + sf2 - sf3;

        int val = - CR;

        val += this->digit(pos1);
        val -= V->digit(pos2);

        if ( val < 0 ) {
            CR = 1;
            val += floatedBigDigit_unit;
        } else {
            CR = 0;
        }

        AA->Val[i] = val;
 
    }

    this->Copy(AA);

    if(CR>0) {
        AA->set(CR);
        AA->shiftPoint = this->shiftPoint + 1;
        this->Add(AA);
    }

    this->isSmall();

    delete AA;

    if(this->checkOver()) return floatedBigDigitERR;

    return floatedBigDigitOK;

}

/****************************************************************************/
/****************************************************************************/

int FloatedBigDigit32::Mul(FloatedBigDigit32* V) {

    if(this->isOver()) {
        return floatedBigDigitERR;
    }

    if(this->isEmpty() || V->isEmpty()) {
        this->set(0);
        return floatedBigDigitOK;
    }

    if(V->isSeed()) {
        int val = V->Val[0];
        bool ret = this->mul(val);
        if(V->isMinus()) this->minus = !this->minus;
        return ret;
    }

    return this->MulSmall(V);

}

/****************************************************************************/

int FloatedBigDigit32::MulSmall(FloatedBigDigit32* V) {


    bool rf = ( this->minus != V->minus );
    this->minus = false;

    this->shiftMax();

    FloatedBigDigit32* BB = new FloatedBigDigit32();
    FloatedBigDigit32* RR = new FloatedBigDigit32();
    FloatedBigDigit32* CC = new FloatedBigDigit32();

    BB->Copy(V);
    BB->shiftMax();

    int sf1 = this->shiftPoint;
    int sf2 = BB->shiftPoint;

    RR->clear();

    for(int i=0;i<=this->N+floatedBigDigit_F;i++) {

        CC->Copy(this);
        CC->mul(BB->Val[i]);
        CC->shiftPoint -= i;

        if(RR->isEmpty()) {
            RR->Copy(CC);
        } else {
            RR->Add(CC);
        }

    }

    RR->minus = rf;

    RR->shiftPoint += sf2;

    this->Copy(RR);

    delete BB;
    delete RR;
    delete CC;

    if(this->checkOver()) return floatedBigDigitERR;

    this->isSmall();

    return floatedBigDigitOK;

}

/****************************************************************************/
/****************************************************************************/

int FloatedBigDigit32::Div(FloatedBigDigit32* V,bool div_fast) {


    if(this->isOver()) {
        return floatedBigDigitERR;
    }

    if(V->isEmpty()) {
        this->overflow();
        return floatedBigDigitERR;
    }

    if(this->isEmpty()) {
        return floatedBigDigitOK;
    }

    if(div_fast && V->isSeed()) {
        int val = V->Val[0];
        bool ret = this->div(val);
        if(V->isMinus()) this->minus = !this->minus;
        return ret;
    }

    return this->DivSmall(V);

}

/****************************************************************************/

int FloatedBigDigit32::DivSmall(FloatedBigDigit32* V) {

    bool rf = !(this->minus == V->minus);
    this->minus = false;

    FloatedBigDigit32* RR = new FloatedBigDigit32();
    FloatedBigDigit32* AA = new FloatedBigDigit32();
    FloatedBigDigit32* BB = new FloatedBigDigit32();
    FloatedBigDigit32* CC = new FloatedBigDigit32();

    CC->Copy(V);
    CC->minus = false;

    // 桁位置調整大
    this->shiftMax();
    CC->shiftMax();
    int sf1 = this->shiftPoint;
    int sf2 = CC->shiftPoint;

    int sfd = sf2 - sf1;
    this->shiftPoint += sfd;

    int idx1,idx2;


    // 桁位置調整小
    for(idx1=1;idx1<floatedBigDigit_unit/10;idx1*=10) {
        if(this->Val[0]*idx1>=floatedBigDigit_unit/10) break;
    }
    for(idx2=1;idx2<floatedBigDigit_unit/10;idx2*=10) {
        if(CC->Val[0]*idx2>=floatedBigDigit_unit/10) break;
    }

    this->mul(idx1);
    CC->mul(idx2);

    AA->set(0);
    BB->set(1);

    int f,s;

    float fv;

    do {

        if(this->Compare(CC)>=0) {

            this->shiftMax();

            s = this->shiftPoint - CC->shiftPoint;

            fv = (float)this->Val[0] + (float)this->Val[1]/floatedBigDigit_unit;
            fv /= ( (float)CC->Val[0] + (float)CC->Val[1]/floatedBigDigit_unit );

            f = (int)fv;

            if(f<=1) { 

                fv = (float)this->Val[0] + (float)this->Val[1]/floatedBigDigit_unit;
                fv /= ( (float)CC->Val[0]/floatedBigDigit_unit );

                f = (int)fv;

                if( f > floatedBigDigit_unit ) f = floatedBigDigit_unit;

                s -= 1;

            }
 
            RR->Copy(CC);
            RR->mul(f);
            RR->shiftPoint += s;
            this->Sub(RR);

            while(this->isMinus()) {
                RR->Copy(this);
                this->Copy(CC);
                this->shiftPoint += s;
                this->Add(RR);
                --f;
            }

            RR->Copy(BB);
            RR->mul(f);
            RR->shiftPoint += s;
            AA->Add(RR);

        }

        this->shiftPoint += 1;
        BB->shiftPoint -= 1;

    } while(!this->isEmpty() && !BB->lastBit());

    AA->shiftPoint -= sfd;

    AA->shiftMax();
    AA->mul(idx2);
    AA->div(idx1);

    AA->minus = rf;

    this->Copy(AA);

    delete RR;
    delete AA;
    delete CC;
    delete BB;

    if(this->checkOver()) return floatedBigDigitERR;

    this->isSmall();

    return floatedBigDigitOK;

}

/****************************************************************************/
/****************************************************************************/

int FloatedBigDigit32::mod(int val) {

    if(val==0) {
        this->overflow();
        return floatedBigDigitERR;
    }

    FloatedBigDigit32* CC = new FloatedBigDigit32();

    CC->set(val);

    bool ret = this->Mod(CC);

    delete CC;

    return ret;

}

/****************************************************************************/

int FloatedBigDigit32::Mod(FloatedBigDigit32* V) {

    if(V->isOver()) {
        this->overflow();
        return floatedBigDigitERR;
    }

    if(V->isZero()) {
        this->overflow();
        return floatedBigDigitERR;
    }

    FloatedBigDigit32* AA = new FloatedBigDigit32();

    FloatedBigDigit32* BB = new FloatedBigDigit32();

    AA->Copy(this);

    BB->Copy(this);

    BB->Div(V);

    BB->Int();

    BB->Mul(V);

    AA->Sub(BB);

    this->Copy(AA);

    AA->Abs();

    BB->Copy(V);

    BB->Abs();

    if(AA->Compare(BB)>=0) this->overflow();

    delete AA;

    delete BB;

    if(this->isOver()) return floatedBigDigitERR;

    return floatedBigDigitOK;

}

/****************************************************************************/
/****************************************************************************/

int FloatedBigDigit32::SetRnd() {

    this->clear();

    this->small = true;

    for(int i=1;i<this->N;i++) {
        float r = (float)rand() / ((float)RAND_MAX + 0.000001);
        this->Val[i] = (int)(floatedBigDigit_unit * r);
    }

    return floatedBigDigitOK;

}

/****************************************************************************/


int FloatedBigDigit32::dice1(int n) {

    this->clear();

    if(n<=0) {
        this->set(0);
        return floatedBigDigitERR;
    }

    this->SetRnd();

    this->mul(n);
    this->add(1);
    this->Fix();

    if(this->isExp()) return floatedBigDigitERR;

    return floatedBigDigitOK;

}

/****************************************************************************/

int FloatedBigDigit32::dice(int a,int b) {

    this->clear();

    FloatedBigDigit32* D = new FloatedBigDigit32();

    for(int i=1;i<=a;i++) {
        D->dice1(b);
        this->Add(D);
    }

    delete D;

    if(this->isExp()) return floatedBigDigitERR;

    return floatedBigDigitOK;

}

/****************************************************************************/

int FloatedBigDigit32::Dice(FloatedBigDigit32* A,FloatedBigDigit32* B) {

    int a = 0;
    int b = 0;

    if(A->isSeed()&&!A->isMinus()) a = A->Val[0];

    if(B->isSeed()&&!B->isMinus()) b = B->Val[0];

    return this->dice(a,b);

}

/****************************************************************************/
/****************************************************************************/

int FloatedBigDigit32::setFactorial(int n) {

    if(n<0) return floatedBigDigitERR;

    FloatedBigDigit32* C = new FloatedBigDigit32();
    C->set(n);

    bool ret = this->SetFactorial(C);

    delete C;

    return ret;

}

/****************************************************************************/

int FloatedBigDigit32::SetFactorial(FloatedBigDigit32* V) {

    if(V->isOver()) {
        this->overflow();
        return floatedBigDigitERR;
    }

    if(V->isMinus()) return floatedBigDigitERR;
    if(V->isSmall()) return floatedBigDigitERR;

    if(V->isZero()) {
        this->set(1);
        return floatedBigDigitOK;
    }

    FloatedBigDigit32* R = new FloatedBigDigit32();
    FloatedBigDigit32* C = new FloatedBigDigit32();

    R->set(1);
    C->Copy(V);
    while(C->compare(1)>0) {
        R->Mul(C);
        C->sub(1);
    }

    this->Copy(R);

    delete R;
    delete C;

    if(this->checkOver()) return floatedBigDigitERR;

    if(this->isExp()) return floatedBigDigitERR;

    return floatedBigDigitOK;

}

/****************************************************************************/

int FloatedBigDigit32::setDoubleFactorial(int n) {

    if(n<0) return floatedBigDigitERR;

    FloatedBigDigit32* C = new FloatedBigDigit32();
    C->set(n);

    bool ret = this->SetDoubleFactorial(C);

    delete C;

    return ret;

}

/****************************************************************************/

int FloatedBigDigit32::SetDoubleFactorial(FloatedBigDigit32* V) {

    if(V->isOver()) {
        this->overflow();
        return floatedBigDigitERR;
    }

    if(V->isMinus()) return floatedBigDigitERR;
    if(V->isSmall()) return floatedBigDigitERR;

    if(V->isZero()) {
        this->set(1);
        return floatedBigDigitOK;
    }

    FloatedBigDigit32* R = new FloatedBigDigit32();
    FloatedBigDigit32* C = new FloatedBigDigit32();

    R->set(1);
    C->Copy(V);

    while(C->compare(1)>0) {
        R->Mul(C);
        C->sub(2);
    }

    this->Copy(R);

    delete R;
    delete C;

    if(this->checkOver()) return floatedBigDigitERR;

    if(this->isExp()) return floatedBigDigitERR;

    return floatedBigDigitOK;

}

/****************************************************************************/

int FloatedBigDigit32::setSequence(int n,int k) {

    if(n<0) return floatedBigDigitERR;
    if(k<0) return floatedBigDigitERR;

    FloatedBigDigit32* N = new FloatedBigDigit32();
    N->set(n);

    FloatedBigDigit32* K = new FloatedBigDigit32();
    K->set(k);

    int ret = this->SetSequence(N,K);

    delete N;
    delete K;

    return ret;

}

/****************************************************************************/

int FloatedBigDigit32::SetSequence(FloatedBigDigit32* N,FloatedBigDigit32* K) {

    if(N->isOver()) {
        this->overflow();
        return floatedBigDigitERR;
    }

    if(K->isOver()) {
        this->overflow();
        return floatedBigDigitERR;
    }

    if(N->isMinus()) return floatedBigDigitERR;
    if(N->isSmall()) return floatedBigDigitERR;
    if(K->isMinus()) return floatedBigDigitERR;
    if(K->isSmall()) return floatedBigDigitERR;

    if(N->isZero()) {
        this->set(0);
        return floatedBigDigitOK;
    }

    if(K->isZero()) {
        this->set(1);
        return floatedBigDigitOK;
     }

    if(K->Compare(N)>0) {
        this->set(0);
        return floatedBigDigitOK;
    }

    FloatedBigDigit32* R = new FloatedBigDigit32();
    FloatedBigDigit32* C = new FloatedBigDigit32();
    FloatedBigDigit32* A = new FloatedBigDigit32();

    A->Copy(N);
    A->Sub(K);

    R->set(1);
    C->Copy(N);

    while(C->Compare(A)>0) {
        R->Mul(C);
        C->sub(1);
    }

    this->Copy(R);

    delete R;
    delete C;
    delete A;

    if(this->checkOver()) return floatedBigDigitERR;

    if(this->isExp()) return floatedBigDigitERR;

    return floatedBigDigitOK;

}

/****************************************************************************/

int FloatedBigDigit32::setCombination(int n,int k) {

    if(n<0) return floatedBigDigitERR;
    if(k<0) return floatedBigDigitERR;

    FloatedBigDigit32* N = new FloatedBigDigit32();
    N->set(n);

    FloatedBigDigit32* K = new FloatedBigDigit32();
    K->set(k);

    int ret = this->SetCombination(N,K);

    delete N;
    delete K;

    return ret;

}

/****************************************************************************/

int FloatedBigDigit32::SetCombination(FloatedBigDigit32* N,FloatedBigDigit32* K) {

    if(N->isOver()) {
        this->overflow();
        return floatedBigDigitERR;
    }

    if(K->isOver()) {
        this->overflow();
        return floatedBigDigitERR;
    }

    if(N->isMinus()) return floatedBigDigitERR;
    if(N->isSmall()) return floatedBigDigitERR;
    if(K->isMinus()) return floatedBigDigitERR;
    if(K->isSmall()) return floatedBigDigitERR;

    if(N->isZero()) {
        this->set(0);
        return floatedBigDigitOK;
    }

    if(K->isZero()) {
        this->set(1);
        return floatedBigDigitOK;
    }

    if(K->Compare(N)>0) {
        this->set(0);
        return floatedBigDigitOK;
    }

    FloatedBigDigit32* R = new FloatedBigDigit32();
    FloatedBigDigit32* B = new FloatedBigDigit32();
    FloatedBigDigit32* C = new FloatedBigDigit32();

    B->Copy(N);
    B->Sub(K);
    B->add(1);
    C->set(1);

    R->set(1);

    do {
        R->Mul(B);
        R->Div(C);
        B->add(1);
        C->add(1);
    } while(C->Compare(K)<=0);

    this->Copy(R);

    delete R;
    delete B;
    delete C;

    if(this->checkOver()) return floatedBigDigitERR;

    if(this->isExp()) return floatedBigDigitERR;

    return floatedBigDigitOK;

}

/****************************************************************************/
/****************************************************************************/

int FloatedBigDigit32::SetE() {

    if(floatedBigDigitCashedE && floatedBigDigitCashE->N == this->N) {
        this->Copy(floatedBigDigitCashE);        
        return floatedBigDigitCashedE_R;
    }

    FloatedBigDigit32* C = new FloatedBigDigit32();

    C->set(1);
    this->set(1);

    int i=0;

    do{
        this->Div(C);
        C->add(1);

    } while(C->compare(floatedBigDigit_LM2) < 0 && !this->lastBit());

    bool stop = ( C->compare(floatedBigDigit_LM2)>=0 );


    this->set(1);
    while(C->compare(1)>0) {
        this->Div(C);
        this->add(1);
        C->sub(1);
    }
    this->add(1);

    delete C;

    if(floatedBigDigitCashedE) delete floatedBigDigitCashE;
    floatedBigDigitCashE =  new FloatedBigDigit32();
    floatedBigDigitCashE->Copy(this);
    floatedBigDigitCashedE = true;
    floatedBigDigitCashedE_R = (stop ? floatedBigDigitERR : floatedBigDigitOK );

    return i;

    if(stop) return floatedBigDigitERR;

    return floatedBigDigitOK;

}

/****************************************************************************/

int FloatedBigDigit32::SetPI() {

    if(floatedBigDigitCashedPI && floatedBigDigitCashPI->N == this->N) {
        this->Copy(floatedBigDigitCashPI);        
        return floatedBigDigitCashedPI_R;
    }

    FloatedBigDigit32* C = new FloatedBigDigit32();

    int ret = C->setAtan2(1,5);
    C->mul(16);
    this->Copy(C);
    C->setAtan2(1,239);
    C->mul(4);
    this->Sub(C);

    if(floatedBigDigitCashedPI) delete floatedBigDigitCashPI;
    floatedBigDigitCashPI =  new FloatedBigDigit32();
    floatedBigDigitCashPI->Copy(this);
    floatedBigDigitCashedPI = true;
    floatedBigDigitCashedPI_R = ret;

    return ret;

}

/****************************************************************************/

int FloatedBigDigit32::setAtan2(int y,int x) {

    if(y == 0) {
        if(x>=0) {
            this->set(0);
        } else {
            this->SetPI();
        }
        return floatedBigDigitOK;
    }

    if(x == 0) {
        this->SetPI();
        if(y>=0) {
            this->div( 2);
        } else {
            this->div(-2);
        }
        return floatedBigDigitOK;
    }

    if(x % y == 0) {
         int d = x/y;
         return this->setAtanDiv(d);
    }

    FloatedBigDigit32* C = new FloatedBigDigit32();
    C->set(y);
    C->div(x);

    int ret = this->SetAtan(C);

    delete C;

    if(x>0) return ret;

    FloatedBigDigit32* P = new FloatedBigDigit32();

    P->SetPI();

    if(y>0) this->Add(P);
    if(y<0) this->Sub(P); 

    delete P;

    return ret;

}


/****************************************************************************/

int FloatedBigDigit32::setAtanDiv(int d) {

    if(d == 0) {
        this->overflow();
        return floatedBigDigitERR;
    }


    bool rf = (d<0);
    if(d<0) d = -d;

    if(d == 1) {
        this->SetPI();
        this->div(4);
        this->minus = rf;
        return floatedBigDigitOK;
    }

    FloatedBigDigit32* C = new FloatedBigDigit32();
    FloatedBigDigit32* P = new FloatedBigDigit32();

    P->set(1);
    this->set(1);

    do {
        P->add(1);
        this->div(d);
        this->div(d);
    } while(P->compare(floatedBigDigit_LM2) < 0 && !this->lastBit());

    bool stop = ( P->compare(floatedBigDigit_LM2)>=0 );

    this->set(1);
    while(P->compare(1)>=0) {
        C->Copy(P);
        C->mul(2);
        C->sub(1);
        this->Mul(C);

        C->Copy(P);
        C->mul(2);
        C->add(1);
        this->Div(C);

        this->div(d);
        this->div(d);

        this->sub(1);
        this->minus = false;

        P->sub(1);
    }

    this->div(d);
    this->minus = rf;

    delete C;
    delete P;

    if(stop) return floatedBigDigitERR;

    return floatedBigDigitOK;

}

/****************************************************************************/
/****************************************************************************/

int FloatedBigDigit32::SetExp(FloatedBigDigit32* V) {

    if(V->isOver()) {
        this->overflow();
        return floatedBigDigitERR;
    }

    FloatedBigDigit32* R = new FloatedBigDigit32();
    FloatedBigDigit32* C = new FloatedBigDigit32();

    if(V->isMinus()) {

        C->Copy(V);
        C->minus = false;

        R->SetExp(C);

        if(R->checkOver()) {
            delete R;
            delete C;
            this->set(0);
            return floatedBigDigitERR;
        }

        this->set(1);
        int ret = this->Div(R);
        delete R;
        delete C;
        return ret;

    }

    if(V->compare(0)==0) {
        this->set(1);
        delete R;
        delete C;
        return floatedBigDigitOK;
    }

    if(V->compare(1)==0) {
        this->SetE();
        delete R;
        delete C;
        return floatedBigDigitOK;
    }

    if(V->compare(1)>0) {


        FloatedBigDigit32* RI = new FloatedBigDigit32();
        FloatedBigDigit32* RF = new FloatedBigDigit32();

        FloatedBigDigit32* I = new FloatedBigDigit32();
        FloatedBigDigit32* F = new FloatedBigDigit32();

        I->Copy(V);
        I->Fix();

        F->Copy(V);
        F->Sub(I);

        RI->SetE();
        RI->Power(I);

        RF->SetExp(F);

        this->Copy(RI);
        this->Mul(RF);

        delete RI;
        delete RF;

        delete I;
        delete F;

        delete R;
        delete C;
           
        return floatedBigDigitOK;

    }


    C->set(1);
    this->set(1);
    do{
        this->Div(C);
        C->add(1);
    } while(C->compare(floatedBigDigit_LMT) < 0 && !this->lastBit());

    bool stop = ( C->compare(floatedBigDigit_LMT)>=0 );

    this->set(1);
    while(C->compare(1)>0) {
        this->Mul(V);
        this->Div(C);
        this->add(1);
        C->sub(1);
    }
    this->Mul(V);
    this->add(1);

    delete R;
    delete C;

    if(stop) return floatedBigDigitERR;

    if(this->checkOver()) return floatedBigDigitERR;

    return floatedBigDigitOK;

}

/****************************************************************************/

int FloatedBigDigit32::SetLn(FloatedBigDigit32* V) {

    if(V->isOver()) {
        this->overflow();
        return floatedBigDigitERR;
    }

    if(V->compare(0) <= 0) {
        this->overflow();
        return floatedBigDigitERR;
    }

    if(V->compare(1) == 0) {
        this->set(0);
        return floatedBigDigitOK;
    }

     if( V->compare(0) > 0 && V->compare(1) < 0 ) {

        FloatedBigDigit32* C = new FloatedBigDigit32();

        C->set(1);
        C->Div(V);

        int ret = this->SetLn(C);
        if(!this->isOver()) this->minus = true;

        delete C;

        return ret;

    }

    FloatedBigDigit32* F = new FloatedBigDigit32();

    F->Copy(V);
    F->mul(100);

    if( F->compare(1000) > 0 ) {
        F->set(2);
    } else if ( F->compare(185) > 0 ) {
        F->set(125);
        F->div(100);
    } else {
        F->Copy(V);
    }

    if( V->Compare(F) > 0 ) {

        FloatedBigDigit32* B = new FloatedBigDigit32();
        B->SetLn(F);

        FloatedBigDigit32* C = new FloatedBigDigit32();
        C->Copy(V);

        this->set(0);
        while(C->Compare(F) > 0) {
            C->Div(F);
            this->Add(B);
        }
        int ret = B->SetLn(C);
        this->Add(B);

        delete F;
        delete B;
        delete C;

        return ret;

    }

    F->Copy(V);
    F->sub(1);

    FloatedBigDigit32* C = new FloatedBigDigit32();
    C->set(1);

    this->set(1);
    do{
        C->add(1);
        this->Mul(F);
    } while(C->compare(floatedBigDigit_LM2) < 0 && !this->lastBit());

    bool stop = ( C->compare(floatedBigDigit_LM2)>=0 );

    this->set(1);
    while(C->compare(1)>0) {
        this->Mul(F);
        C->sub(1);
        this->Mul(C);
        C->add(1);
        this->Div(C);
        C->sub(1);
        this->sub(1);
        this->minus = false;
    }

    this->Mul(F);

    delete F;
    delete C;

    if(stop) return floatedBigDigitERR;

    return floatedBigDigitOK;

}

/****************************************************************************/


int FloatedBigDigit32::SetLog(FloatedBigDigit32* V) {

    if(V->isOver()) {
        this->overflow();
        return floatedBigDigitERR;
    }

    if(V->compare(0) <= 0) {
        this->overflow();
        return floatedBigDigitERR;
    }

    if(V->compare(1) == 0) {
        this->set(0);
        return floatedBigDigitOK;
    }

    int ret = this->SetLn(V);

    FloatedBigDigit32* T = new FloatedBigDigit32();
    FloatedBigDigit32* F = new FloatedBigDigit32();

    T->set(10);
    F->SetLn(T);

    this->Div(F);

    delete T;
    delete F;

    return ret;

}

/****************************************************************************/

int FloatedBigDigit32::SetLog2(FloatedBigDigit32* V) {

    if(V->isOver()) {
        this->overflow();
        return floatedBigDigitERR;
    }

    if(V->compare(0) <= 0) {
        this->overflow();
        return floatedBigDigitERR;
    }

    if(V->compare(1) == 0) {
        this->set(0);
        return floatedBigDigitOK;
    }

    int ret = this->SetLn(V);

    FloatedBigDigit32* T = new FloatedBigDigit32();
    FloatedBigDigit32* F = new FloatedBigDigit32();

    T->set(2);
    F->SetLn(T);

    this->Div(F);

    delete T;
    delete F;

    return ret;

}


/****************************************************************************/

int FloatedBigDigit32::power(int n) {

    if(n==0) {
        this->set(1);
        return floatedBigDigitOK;
    }

    FloatedBigDigit32* C = new FloatedBigDigit32();

    C->set(n);

    int ret = this->Power(C);

    delete C;

    return ret;

}

/****************************************************************************/

int FloatedBigDigit32::Power(FloatedBigDigit32* V) {

    if(this->isOver()) {
        this->overflow();
        return floatedBigDigitERR;
    }

    if(V->isOver()) {
        this->overflow();
        return floatedBigDigitERR;
    }

    if(this->isMinus() && V->isSmall()) {
        this->overflow();
        return floatedBigDigitERR;
    } 

    FloatedBigDigit32* A = new FloatedBigDigit32();

    FloatedBigDigit32* F = new FloatedBigDigit32();

    F->Copy(V);

    F->Abs();

    bool conv = ( this->isSmall() || F->compare(1000)>0 );

    F->set(1);

    if(this->isMinus()) {

        this->Sig();

        A->Copy(V);
        A->Abs();
        A->mod(2);

        if(A->compare(0)>0) F->Sig();

    }

    if(conv) {

        A->SetLog(this);

        A->Mul(V);

        this->set(10);

    } else {

        A->Copy(V);

    }

    int ret = this->Power_main(A,true);

    this->Mul(F);

    delete A;

    delete F;

    return ret;

}

/****************************************************************************/

int FloatedBigDigit32::Power_main(FloatedBigDigit32* V,bool boost) {


    if(this->isEmpty()) {
        return floatedBigDigitOK;
    }

    if(V->isZero()) {
        this->set(1);
        return floatedBigDigitOK;
    }

    bool sf = this->isSmall();

    // LOGIC MINUS
    if(V->isMinus()) {

        FloatedBigDigit32* R = new FloatedBigDigit32();
        FloatedBigDigit32* C = new FloatedBigDigit32();

        C->Copy(V);
        C->minus = false;

        R->Copy(this);
        R->Power(C);

        if(R->isOver()) {
            this->set(0);
        } else {
            this->set(1);
            this->Div(R);
        }

        delete R;
        delete C;

        return floatedBigDigitOK;

    }

    // LOGIC REAL
    if(V->isSmall()) {

        if(this->isMinus()) {
            this->overflow();
            return floatedBigDigitOK;
        }

        FloatedBigDigit32* RI = new FloatedBigDigit32();
        FloatedBigDigit32* RF = new FloatedBigDigit32();

        FloatedBigDigit32* I = new FloatedBigDigit32();
        FloatedBigDigit32* F = new FloatedBigDigit32();


        I->Copy(V);
        I->Fix();

        F->Copy(V);
        F->Sub(I);

        // INTEGER PART
        RI->Copy(this);
        RI->Power(I);

        if(RI->isOver()) {

            this->overflow();

            delete RI;
            delete RF;

            delete I;
            delete F;

            return floatedBigDigitERR;

        }

        FloatedBigDigit32* A = new FloatedBigDigit32();

        // SMALL CHECK
        A->set(1);
        A->Div(F);
        A->FR();

        if(A->isOver()) A->set(0);
        if(A->isSmall()) A->set(0);
        if(A->compare(100)>0) A->set(0);

        if( boost && !A->isZero()) {

            // LOGIG FOR 1/M
            RF->Copy(this);
            RF->PowerDiv(A);

        } else {

            // LOGIC FOR REAL
            A->SetLn(this);
            F->Mul(A);

            RF->SetExp(F);

        }

        // MERGE RESULT
        this->Copy(RI);
        this->Mul(RF);

        delete RI;
        delete RF;

        delete I;
        delete F;

        delete A;

        if(this->checkOver()) return floatedBigDigitERR;

        return floatedBigDigitOK;

    }

    // LOGIC INTEGER

    FloatedBigDigit32* B = new FloatedBigDigit32();
    FloatedBigDigit32* C = new FloatedBigDigit32();

    B->Copy(this);
    C->Copy(V);
    C->minus = false;

    this->set(1);
    while(!C->isZero()) {
        this->Mul(B);
        if(this->isOver()) break;
        C->sub(1);
    }

    if(V->isMinus()) {
        if(this->isOver()) {
            this->set(0);
        } else {
            B->Copy(this);
            this->set(1);
            this->Div(B);
        }
    }

    delete B;
    delete C;


    if(this->isOver()) return floatedBigDigitERR;

    return floatedBigDigitOK;

}

/****************************************************************************/

int FloatedBigDigit32::PowerDiv(FloatedBigDigit32* V) {

    if(this->isOver()) {
        this->overflow();
        return floatedBigDigitERR;
    }

    if(V->isOver()) {
        this->overflow();
        return floatedBigDigitERR;
    }

    if(this->isZero()) {
        this->clear();
        return floatedBigDigitOK;
    }

    if(this->isMinus()) {
        this->overflow();
        return floatedBigDigitERR;
    }

    if(V->isSmall()) {
        this->overflow();
        return floatedBigDigitERR;
    }


    FloatedBigDigit32* F = new FloatedBigDigit32();
    FloatedBigDigit32* A = new FloatedBigDigit32();

    A->set(1);

    int p;

    do {

       p = this->PowerDiv_boost(V);
       if(p<=1) break;

       F->set(p);
       F->Power_main(V,false);

       this->Div(F);
       A->mul(p);

    } while (p>1);

    F->set(1);
    F->Div(V);

    this->Power_main(F,false);
    this->Mul(A);

    delete F;
    delete A;

    return floatedBigDigitOK;

}

/****************************************************************************/

int FloatedBigDigit32::PowerDiv_boost(FloatedBigDigit32* V) {


    if(this->isSmall()) return -1;

    FloatedBigDigit32* R = new FloatedBigDigit32();
    FloatedBigDigit32* F = new FloatedBigDigit32();
    FloatedBigDigit32* I = new FloatedBigDigit32();

    I->set(2);

    int ret = -1;

    do {
        R->Copy(I);
        R->Power(V);
        if(this->Compare(R)<0) break;
        F->Copy(this);
        F->Div(R);
        if(!F->isSmall()) {
            ret = I->Val[0];
            break;
        }
        I->add(1);
    } while(I->compare(100)<0);

    delete R;
    delete F;
    delete I;

    return ret;

}

/****************************************************************************/

int FloatedBigDigit32::Sqrt() {

    if(this->isOver()) {
        this->overflow();
        return floatedBigDigitERR;
    }

    if(this->isZero()) {
        this->clear();
        return floatedBigDigitOK;
    }

    if(this->isMinus()) {
        this->overflow();
        return floatedBigDigitERR;
    }

    FloatedBigDigit32* F = new FloatedBigDigit32();

    F->set(2);

    int ret = this->PowerDiv(F);

    delete F;

    return ret;

} 

/****************************************************************************/
/****************************************************************************/

int FloatedBigDigit32::SetSin(FloatedBigDigit32* V) {

    if(V->isOver()) {
        this->overflow();
        return floatedBigDigitERR;
    }

    FloatedBigDigit32* P = new FloatedBigDigit32();
    P->SetPI();

    FloatedBigDigit32* P2 = new FloatedBigDigit32();
    P2->SetPI();
    P2->mul(2);

    FloatedBigDigit32* F = new FloatedBigDigit32();
    F->Copy(V);
    bool rf = F->isMinus();
    F->minus = false;

    if(F->Compare(P2)>=0) {
        F->Mod(P2);
    }

    if(F->Compare(P)>=0) {
        F->Sub(P);
        F->Sub(P);
    }

    if(F->isMinus()) rf = !rf;
    F->minus = false;

    P->div(2);
    if(F->Compare(P)>=0) {
        F->Sub(P);
        F->Sub(P);
        F->minus = false;
    }

    P->div(2);
   if(F->Compare(P)>0) {

        F->Sub(P);
        F->Sub(P);
        F->minus = false;
        this->SetCos(F);
        this->minus = rf;

        delete P;
        delete P2;
        delete F;

        return floatedBigDigitOK;

    }

    if(F->isEmpty()) {

        delete P;
        delete P2;
        delete F;

        this->set(0);
        return floatedBigDigitOK;

    }

    FloatedBigDigit32* C = new FloatedBigDigit32();

    C->set(1);
    this->set(1);

    do{
        this->Mul(F);
        this->Mul(F);
        C->add(1);
        this->Div(C);
        C->add(1);
        this->Div(C);
    } while(C->compare(floatedBigDigit_LM2) < 0 && !this->lastBit());

    bool stop = ( C->compare(floatedBigDigit_LM2)>=0 );

    this->set(1);
    while(C->compare(1)>0) {
        this->Mul(F);
        this->Mul(F);
        this->Div(C);
        C->sub(1);
        this->Div(C);
        C->sub(1);
        this->sub(1);
        this->minus = false;
    }

    this->Mul(F);

    delete P;
    delete P2;
    delete F;
    delete C;

    this->minus = rf;

    if(stop) return floatedBigDigitERR;

    return floatedBigDigitOK;

}

/****************************************************************************/

int FloatedBigDigit32::SetCos(FloatedBigDigit32* V) {

    if(V->isOver()) {
        this->overflow();
        return floatedBigDigitERR;
    }

    FloatedBigDigit32* P = new FloatedBigDigit32();
    P->SetPI();

    FloatedBigDigit32* P2 = new FloatedBigDigit32();
    P2->SetPI();
    P2->mul(2);

    FloatedBigDigit32* F = new FloatedBigDigit32();
    F->Copy(V);
    bool rf = false;
    F->minus = false;


    if(F->Compare(P2)>=0) {
        F->Mod(P2);
    }

    if(F->Compare(P)>=0) {
        F->Sub(P);
        F->Sub(P);
    }
    F->minus = false;

    P->div(2);
    if(F->Compare(P)>=0) {
        rf = true;
        F->Sub(P);
        F->Sub(P);
        F->minus = false;
    }

    P->div(2);
    if(F->Compare(P)>0) {

        F->Sub(P);
        F->Sub(P);
        F->minus = false;
        int ret = this->SetSin(F);
        this->minus = rf;

        delete P;
        delete P2;
        delete F;

        return ret;
    }

    if(F->isEmpty()) {

        this->set(1);

        delete P;
        delete P2;
        delete F;

        return floatedBigDigitOK;

    }

    FloatedBigDigit32* C = new FloatedBigDigit32();

    this->set(1);
    C->set(2);
    this->Div(C);
    do{
        this->Mul(F);
        this->Mul(F);
        C->add(1);
        this->Div(C);
        C->add(1);
        this->Div(C);
    } while(C->compare(floatedBigDigit_LM2) < 0 && !this->lastBit());

    bool stop = ( C->compare(floatedBigDigit_LM2)>=0 );

    this->set(1);
    while(C->compare(2)>=0) {
        this->Mul(F);
        this->Mul(F);
        this->Div(C);
        C->sub(1);
        this->Div(C);
        C->sub(1);
        this->sub(1);
        this->minus = false;
    }

    this->minus = rf;

    delete P;
    delete P2;
    delete F;
    delete C;


    if(stop) return floatedBigDigitERR;

    return floatedBigDigitOK;

}

/****************************************************************************/

int FloatedBigDigit32::SetTan(FloatedBigDigit32* V) {

    if(V->isOver()) {
        this->overflow();
        return floatedBigDigitERR;
    }

    FloatedBigDigit32* C = new FloatedBigDigit32();

    this->SetSin(V);

    C->SetCos(V);

    this->Div(C);

    delete C;

    if(this->checkOver()) return floatedBigDigitERR;

    return floatedBigDigitOK;

}


/****************************************************************************/
/****************************************************************************/

int FloatedBigDigit32::SetSinh(FloatedBigDigit32* V) {

    if(V->isOver()) {
        this->overflow();
        return floatedBigDigitERR;
    }

    if(V->isEmpty()) {
        this->set(0);
        return floatedBigDigitOK;
    }

    FloatedBigDigit32* F = new FloatedBigDigit32();
    FloatedBigDigit32* C = new FloatedBigDigit32();

    F->Copy(V);
    bool rf = F->isMinus();
    F->minus = false;

    this->Copy(F);

    C->set(1);
    do{
        this->Mul(F);
        C->add(1);
        this->Div(C);
        this->Mul(F);
        C->add(1);
        this->Div(C);
    } while(C->compare(floatedBigDigit_LM2) < 0 && !this->lastBit());

    if( this->isOver() || this->compare(1) > 0 ) {

        delete F;
        delete C;

        return this->SetSinhB(V);

    }

    bool stop = ( C->compare(floatedBigDigit_LM2)>=0 );

    this->set(1);
    while(C->compare(1)>0) {

        this->Mul(F);

        this->Div(C);
        C->sub(1);
        this->Mul(F);
        this->Div(C);
        C->sub(1);

        this->add(1);

    }

    this->Mul(F);

    this->minus = rf;

    delete F;
    delete C;

    if(this->checkOver()) return floatedBigDigitERR;

    if(stop) return floatedBigDigitERR;

    return floatedBigDigitOK;

}

/****************************************************************************/

int FloatedBigDigit32::SetSinhB(FloatedBigDigit32* V) {

    if(V->isOver()) {
        this->overflow();
        return floatedBigDigitERR;
    }

    FloatedBigDigit32* F = new FloatedBigDigit32();

    F->Copy(V);

    bool rf = F->minus;
    F->minus = false;

    FloatedBigDigit32* A = new FloatedBigDigit32();
    FloatedBigDigit32* B = new FloatedBigDigit32();

    A->SetExp(F);

    B->set(1);
    B->Div(A);

    this->Copy(A);
    this->Sub(B);
    this->div(2);

    if(!this->isOver()) this->minus = rf;

    delete F;
    delete A;
    delete B;

    if(this->isOver())return floatedBigDigitERR;

    return floatedBigDigitOK;

}

/****************************************************************************/

int FloatedBigDigit32::SetCosh(FloatedBigDigit32* V) {

    if(V->isOver()) {
        this->overflow();
        return floatedBigDigitERR;
    }

    if(V->isEmpty()) {
        this->set(1);
        return floatedBigDigitOK;
    }

    FloatedBigDigit32* C = new FloatedBigDigit32();

    this->Copy(V);
    this->minus = false;

    C->set(0);
    do{
        this->Mul(V);
        C->add(1);
        this->Div(C);
        this->Mul(V);
        C->add(1);
        this->Div(C);
    } while(C->compare(floatedBigDigit_LM2) < 0 && !this->lastBit());

    if( this->isOver() || this->compare(1) > 0 ) {

        delete C;

        return this->SetCoshB(V);

    }

    bool stop = ( C->compare(floatedBigDigit_LM2)>=0 );

    this->set(1);
    while(C->compare(1)>0) {

        this->Mul(V);
        this->Div(C);
        C->sub(1);
        this->Mul(V);
        this->Div(C);
        C->sub(1);

        this->add(1);

    }

    delete C;

    if(this->checkOver()) return floatedBigDigitERR;

    if(stop) return floatedBigDigitERR;

    return floatedBigDigitOK;

}

/****************************************************************************/

int FloatedBigDigit32::SetCoshB(FloatedBigDigit32* V) {

    if(V->isOver()) {
        this->overflow();
        return floatedBigDigitERR;
    }

    FloatedBigDigit32* F = new FloatedBigDigit32();

    F->Copy(V);

    F->minus = false;

    FloatedBigDigit32* A = new FloatedBigDigit32();
    FloatedBigDigit32* B = new FloatedBigDigit32();

    A->SetExp(F);

    B->set(1);
    B->Div(A);

    this->Copy(A);
    this->Add(B);
    this->div(2);

    delete F;
    delete A;
    delete B;

    if(this->isOver())return floatedBigDigitERR;

    return floatedBigDigitOK;

}

/****************************************************************************/

int FloatedBigDigit32::SetTanh(FloatedBigDigit32* V) {

    if(V->isOver()) {
        this->overflow();
        return floatedBigDigitERR;
    }

    FloatedBigDigit32* C = new FloatedBigDigit32();

    int ret = this->SetSinh(V);

    C->SetCosh(V);

    this->Div(C);

    delete C;

    return ret;

}


/****************************************************************************/

int FloatedBigDigit32::SetAtan2(FloatedBigDigit32* Y, FloatedBigDigit32* X) {

    if(Y->isOver()) {
        this->overflow();
        return floatedBigDigitERR;
    }

    if(X->isOver()) {
        this->overflow();
        return floatedBigDigitERR;
    }

    if(Y->isEmpty()) {
        if(X->compare(0)>=0) {
            this->set(0);
        } else {
            this->SetPI();
        }
        return floatedBigDigitOK;
    }

    if(X->isEmpty()) {
        this->SetPI();
        if(Y->compare(0)>=0) {
            this->div( 2);
        } else {
            this->div(-2);
        }
        return floatedBigDigitOK;
    }

    FloatedBigDigit32* C = new FloatedBigDigit32();

    if(!Y->isSmall()) {

        C->Copy(X);
        C->Div(Y);

        if(!C->isSmall() && C->isSeed()) {
            int ret = this->setAtanDiv(C->digit(0));
            delete C;
            return ret;
        }
    }

    C->Copy(Y);
    C->Div(X);

    int ret = this->SetAtan(C);

    delete C;

    if(X->compare(0)>0) return ret;

    FloatedBigDigit32* P = new FloatedBigDigit32();
    P->SetPI();

   if(Y->compare(0)>0) this->Add(P);
   if(Y->compare(0)<0) this->Sub(P);

    delete P;

    return ret;

}

/****************************************************************************/

int FloatedBigDigit32::SetAtan(FloatedBigDigit32* V) {

    if(V->isOver()) {
        this->overflow();
        return floatedBigDigitERR;
    }

    if(V->compare(0)==0) {
        this->set(0);
        return floatedBigDigitOK;
    }

    FloatedBigDigit32* C = new FloatedBigDigit32();

    if(V->isMinus()) {
        C->Copy(V);
        C->minus = false;
        int ret = this->SetAtan(C);
        this->minus = true;
        delete C;
        return ret;
    }

    FloatedBigDigit32* P = new FloatedBigDigit32();

    P->SetPI();
    P->div(2);

    if(V->compare(1)==0) {
        P->div(2);
        this->Copy(P);
        delete C;
        delete P;
        return floatedBigDigitOK;
    }

    if(V->compare(1)>0) {
        C->set(1);
        C->Div(V);
        int ret = this->SetAtan(C);
        this->Sub(P);
        this->minus = false;
        delete C;
        delete P;
        return ret;
    }

    bool rf = V->isMinus();
    V->minus = false;

    P->set(0);
    C->Copy(V);
    do {
        C->Mul(V);        
        C->Mul(V);
        P->add(1);
    } while(P->compare(floatedBigDigit_LM2) < 0 && !C->lastBit());

    bool stop = ( P->compare(floatedBigDigit_LM2)>=0 );

    if(stop) {
        V->minus = rf;
        delete C;
        delete P;
        return this->SetAtan_boost(V);
    }

    this->set(1);
    while(P->compare(1)>=0) {

        C->Copy(P);
        C->mul(2);
        C->sub(1);
        this->Mul(C);

        this->Mul(V);
        this->Mul(V);

        C->Copy(P);
        C->mul(2);
        C->add(1);
        this->Div(C);

        this->sub(1);
        this->minus = false;

        P->sub(1);
    }

    this->Mul(V);

    this->minus = rf;
    V->minus = rf;

    delete C;
    delete P;

    if(stop) return floatedBigDigitERR;

    return floatedBigDigitOK;

}

/****************************************************************************/

int FloatedBigDigit32::SetAtan_boost(FloatedBigDigit32* V) {

    FloatedBigDigit32* F = new FloatedBigDigit32();
    FloatedBigDigit32* A = new FloatedBigDigit32();
    FloatedBigDigit32* B = new FloatedBigDigit32();

    F->Copy(V);

    A->Copy(V);
    A->Mul(V);
    A->add(1);

    B->set(2);
    A->PowerDiv(B);
    A->add(1);

    F->Div(A);

    int ret = this->SetAtan(F);
    this->mul(2);

    delete F;
    delete A;
    delete B;

    return ret;

}

/****************************************************************************/

int FloatedBigDigit32::SetAsin(FloatedBigDigit32* V) {

    if(V->isOver()) {
        this->overflow();
        return floatedBigDigitERR;
    }

    FloatedBigDigit32* F = new FloatedBigDigit32();

    F->Copy(V);

    bool rf = F->minus;
    F->minus = false;

    if(F->compare(1)>0) {
        this->overflow();
        delete F;
        return floatedBigDigitERR;
    }

    if(F->compare(1)==0) {
        this->SetPI();
        this->div(2);
        this->minus = rf;
        delete F;
        return floatedBigDigitOK;
    }


    FloatedBigDigit32* C = new FloatedBigDigit32();

    C->set(1);

    this->Copy(F);
    while(C->compare(floatedBigDigit_LM2) < 0 && !this->lastBit()) {
        this->Mul(F);
        this->Mul(F);

        this->Mul(C);
        C->add(1);
        this->Div(C);
        C->add(1);
    }

    bool stop = ( C->compare(floatedBigDigit_LM2)>=0 );

    if(stop) {
        delete F;
        delete C;
        return this->SetAsin_boost(V);
    }

    this->set(1);
    while(C->compare(1)>=0) {

        this->Mul(F);

        this->Mul(C);
        C->add(1);
        this->Div(C);
        C->sub(1);

        this->Mul(F);
        this->Mul(C);
        C->add(2);
        this->Div(C);
        C->sub(4);

        this->add(1);

    }

    this->Mul(F);

    this->minus = rf;

    delete F;
    delete C;

    if(stop) return floatedBigDigitERR;

    return floatedBigDigitOK;

}

/****************************************************************************/

int FloatedBigDigit32::SetAsin_boost(FloatedBigDigit32* V) {

    FloatedBigDigit32* F = new FloatedBigDigit32();
    FloatedBigDigit32* A = new FloatedBigDigit32();
    FloatedBigDigit32* B = new FloatedBigDigit32();

    F->Copy(V);

    A->Copy(V);
    A->Mul(V);
    A->sub(1);
    A->Sig();

    B->set(2);
    A->PowerDiv(B);
    A->add(1);

    F->Div(A);

    int ret = this->SetAtan(F);
    this->mul(2);

    delete F;
    delete A;
    delete B;

    return ret;

}

/****************************************************************************/

int FloatedBigDigit32::SetAcos(FloatedBigDigit32* V) {

    if(V->isOver()) {
        this->overflow();
        return floatedBigDigitERR;
    }

    FloatedBigDigit32* F = new FloatedBigDigit32();

    F->Copy(V);

    bool rf = F->minus;
    F->minus = false;

    if(F->compare(1)>0) {
        this->overflow();
        delete F;
        return floatedBigDigitERR;
    }

    if(F->compare(1)==0) {
        if(rf) {
            this->SetPI();
        } else {
            this->set(0);
        }
        delete F;
        return floatedBigDigitOK;
    }

    int ret = this->SetAsin(F);

    F->SetPI();
    F->div(2);

    this->Sub(F);
    this->minus = false;

    if(rf) {
        F->SetPI();
        F->Sub(this);
        this->Copy(F);
    }


    delete F;

    return ret;

}

/****************************************************************************/
/****************************************************************************/

int FloatedBigDigit32::SetAsinh(FloatedBigDigit32* V) {

    if(V->isOver()) {
        this->overflow();
        return floatedBigDigitERR;
    }

    this->Copy(V);
    this->minus = false;

    if(this->compare(1)>=0) {
        return this->SetAsinhR(V);
    }

    FloatedBigDigit32* C = new FloatedBigDigit32();

    C->set(1);

    bool rf = V->minus;
    V->minus = false;

    do{

        this->Mul(V);
        this->Mul(V);

        this->Mul(C);
        C->add(1);
        this->Div(C);
        C->add(1);

    } while(C->compare(floatedBigDigit_LM2) < 0 && !this->lastBit());

    bool stop = ( C->compare(floatedBigDigit_LM2)>=0 );

    if(stop) {
        V->minus = rf;
        delete C;
        return this->SetAsinhB(V);
    }


    this->set(1);
    while(C->compare(1)>=0) {

        this->Mul(C);
        C->add(1);
        this->Mul(V);
        this->Div(C);
        C->sub(1);
        this->Mul(C);
        C->add(2);
        this->Mul(V);
        this->Div(C);
        C->sub(4);

        this->sub(1);
        this->minus = false;

    }

    this->Mul(V);
    this->minus = rf;
    V->minus = rf;

    delete C;

    if(stop) return floatedBigDigitERR;

    return floatedBigDigitOK;

}

/****************************************************************************/

int FloatedBigDigit32::SetAsinhR(FloatedBigDigit32* V) {

    if(V->isOver()) {
        this->overflow();
        return floatedBigDigitERR;
    }

    FloatedBigDigit32* F = new FloatedBigDigit32();

    F->set(1);
    F->Div(V);
    F->Abs();

    bool rf = V->isMinus();

    if(F->compare(1)>0) {
        delete F;
        return this->SetAsinh(V);
    }

    FloatedBigDigit32* C = new FloatedBigDigit32();

    C->set(0);
    this->set(1);

    do{
        this->Mul(F);
        this->Mul(F);

        C->add(1);
        this->Mul(C);
        C->add(1);
        this->Div(C);

    } while(C->compare(floatedBigDigit_LM2) < 0 && !this->lastBit());

    bool stop = ( C->compare(floatedBigDigit_LM2)>=0 );

    if(stop) {
        delete F;
        delete C;
        return this->SetAsinhB(V);
    }

    this->set(1);
    while(C->compare(4)>=0) {

        this->Mul(F);

        C->sub(1);
        this->Mul(C);
        C->add(1);
        this->Div(C);

        this->Mul(F);
          
        C->sub(2);
        this->Mul(C);
        C->add(2);
        this->Div(C);

        C->sub(2);

        this->sub(1);
        this->minus = false;

    }

    this->Mul(F);
    this->Mul(F);
    this->div(4);

    FloatedBigDigit32* B = new FloatedBigDigit32();

    B->Copy(V);
    B->Abs();
    C->SetLn(B);
    this->Add(C);

    B->set(2);
    C->SetLn(B);
    this->Add(C);

    this->minus = rf;

    delete F;
    delete C;
    delete B;

    if(stop) return floatedBigDigitERR;

    return floatedBigDigitOK;

}

/****************************************************************************/

int FloatedBigDigit32::SetAsinhB(FloatedBigDigit32* V) {

    if(V->isOver()) {
        this->overflow();
        return floatedBigDigitERR;
    }

    FloatedBigDigit32* F = new FloatedBigDigit32();
    FloatedBigDigit32* A = new FloatedBigDigit32();
    FloatedBigDigit32* B = new FloatedBigDigit32();

    F->Copy(V);
    F->Mul(V);

    A->Copy(F);
    A->add(1);

    B->set(2);

    A->PowerDiv(B);
    A->add(1);

    F->Div(A);

    B->Copy(V);
    B->minus = false;

    if(F->isOver()) F->Copy(B);

    F->add(1);

    F->Add(B);

    this->SetLn(F);

    delete F;
    delete A;
    delete B;

    if(this->isOver()) return floatedBigDigitERR;

    if(V->isMinus()) this->minus = true;

    return floatedBigDigitOK;

}

/****************************************************************************/

int FloatedBigDigit32::SetAcosh(FloatedBigDigit32* V) {

    if(V->isOver()) {
        this->overflow();
        return floatedBigDigitERR;
    }

    if(V->isMinus()) {
        this->overflow();
        return floatedBigDigitERR;
    }

    if(V->compare(1)==0) {
        this->set(0);
        return floatedBigDigitOK;
    }

    FloatedBigDigit32* F = new FloatedBigDigit32();

    F->set(1);
    F->Div(V);
    F->Abs();

    if(F->compare(1)>=0) {
        this->overflow();
        delete F;
        return floatedBigDigitERR;
    }

    FloatedBigDigit32* C = new FloatedBigDigit32();
 
    C->set(0);
    this->set(1);

    do{

        this->Mul(F);
        this->Mul(F);

        C->add(1);
        this->Mul(C);
        C->add(1);
        this->Div(C);

    } while(C->compare(floatedBigDigit_LM2) < 0 && !this->lastBit());

    bool stop = ( C->compare(floatedBigDigit_LM2)>=0 );

    if(stop) {
        delete F;
        delete C;
        return this->SetAcoshB(V);
    }

    this->set(1);
    while(C->compare(4)>=0) {

        this->Mul(F);

        C->sub(1);
        this->Mul(C);
        C->add(1);
        this->Div(C);

        this->Mul(F);
          
        C->sub(2);
        this->Mul(C);
        C->add(2);
        this->Div(C);

        C->sub(2);

        this->add(1);

    }

    this->Mul(F);
    this->Mul(F);
    this->div(4);

    FloatedBigDigit32* B = new FloatedBigDigit32();

    B->Copy(V);
    C->SetLn(B);

    B->set(2);
    F->SetLn(B);
    C->Add(F);

    C->Sub(this);

    delete F;
    delete C;
    delete B;

    this->Copy(C);

    if(stop) return floatedBigDigitERR;

    return floatedBigDigitOK;

}

/****************************************************************************/

int FloatedBigDigit32::SetAcoshB(FloatedBigDigit32* V) {

    if(V->isOver()) {
        this->overflow();
        return floatedBigDigitERR;
    }

    if(V->isMinus()) {
        this->overflow();
        return floatedBigDigitERR;
    }

    if(V->compare(1)==0) {
        this->set(0);
        return floatedBigDigitOK;
    }

    FloatedBigDigit32* A = new FloatedBigDigit32();
    FloatedBigDigit32* B = new FloatedBigDigit32();

    A->Copy(V);
    A->Mul(V);
    A->sub(1);

    B->set(2);
    A->PowerDiv(B);

    if(A->isOver()) A->Copy(V);

    A->Add(V);

    this->SetLn(A);

    delete A;
    delete B;

    if(this->isOver()) return floatedBigDigitERR;

    return floatedBigDigitOK;

}

/****************************************************************************/

int FloatedBigDigit32::SetAtanh(FloatedBigDigit32* V) {

    if(V->isOver()) {
        this->overflow();
        return floatedBigDigitERR;
    }

    bool rf = V->minus;
    V->minus = false;
    this->Copy(V);

    if(this->compare(1)>=0) {
        this->overflow();
        V->minus = rf;
        return floatedBigDigitERR;
    }

    FloatedBigDigit32* C = new FloatedBigDigit32();

    C->set(1);

    do{
        this->Mul(V);
        this->Mul(V);
        C->add(2);
    } while(C->compare(floatedBigDigit_LM2) < 0 && !this->lastBit());

    bool stop = ( C->compare(floatedBigDigit_LM2)>=0 );


    if(stop) {
        delete C;
        return this->SetAtanhB(V);
    }

    this->set(1);
    while(C->compare(1)>=0) {

        this->Mul(V);
        this->Mul(V);

        this->Mul(C);
        C->add(2);
        this->Div(C);
        C->sub(4);

        this->add(1);

    }

    this->Mul(V);
    this->minus = rf;
    V->minus = rf;

    delete C;
    return floatedBigDigitOK;

}

/****************************************************************************/

int FloatedBigDigit32::SetAtanhB(FloatedBigDigit32* V) {

    if(V->isOver()) {
        this->overflow();
        return floatedBigDigitERR;
    }

    FloatedBigDigit32* F = new FloatedBigDigit32();

    F->Copy(V);

    bool rf = F->minus;
    F->minus = false;

    if(F->compare(1)>=0) {
        this->overflow();
        delete F;
        return floatedBigDigitERR;
    }

    FloatedBigDigit32* A = new FloatedBigDigit32();
    FloatedBigDigit32* B = new FloatedBigDigit32();

    A->set(1);
    A->Add(F);
    B->set(1);
    B->Sub(F);

    A->Div(B);

    this->SetLn(A);
    this->div(2);

    if(!this->isOver()) this->minus = rf;

    delete F;
    delete A;
    delete B;

    if(this->isOver())return floatedBigDigitERR;

    return floatedBigDigitOK;

}

/****************************************************************************/
