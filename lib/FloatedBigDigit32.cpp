/* FLOATED BIG DIGIT CLASS */
/* CREATE  2021.02.06      */
/* REVISED 2021.05.11      */
/* Ver 0.113  ( U.C. )     */
/* Original by K-ARAI      */

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>

#include "FloatedBigDigit32.h"

/****************************************************************************/

int floatedBigDigit_K     = 5;
int floatedBigDigit_unit = 100000;

int floatedBigDigit_F     = 2;
int floatedBigDigit_LMT = 12000;
int floatedBigDigit_LM2 = 30000;

int floatedBigDigitERR = -1;
int floatedBigDigitOK = 0;

/****************************************************************************/

int floatedBigDigit_keta = 100;

/****************************************************************************/

bool floatedBigDigitCashedE = false;
FloatedBigDigit32* floatedBigDigitCashE;
int floatedBigDigitCashedE_R;

bool floatedBigDigitCashedPI = false;
FloatedBigDigit32* floatedBigDigitCashPI;
int floatedBigDigitCashedPI_R;

/****************************************************************************/
/****************************************************************************/

int SetRandomSeed(int seed) {

    srand(seed);

    return floatedBigDigitOK;
}

/****************************************************************************/

int SetFloatedBigDigit32Keta(int k) {

    if(k<=0) return floatedBigDigitERR;

    floatedBigDigit_keta = k;

    return floatedBigDigitOK;

}

/****************************************************************************/

int FreeFloatedBigDigit32() {


    if(floatedBigDigitCashedE ) delete floatedBigDigitCashE;

    if(floatedBigDigitCashedPI) delete floatedBigDigitCashPI;

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

bool FloatedBigDigit32::lastBit() {

        int last_bit_boost = 3;

        if(!this->isSmall()) return false;

        FloatedBigDigit32* CC = new FloatedBigDigit32();

        CC->set(1);

        CC->shiftPoint = - ( CC->N + floatedBigDigit_F*last_bit_boost );

        bool ret = !(this->Compare(CC)>=0);

        delete CC;

        return ret;

}

/****************************************************************************/

int FloatedBigDigit32::Copy(FloatedBigDigit32* V) {

        if( this->N != V->N ) return floatedBigDigitERR;

        for(int i=0;i<=this->N+floatedBigDigit_F;i++) this->Val[i] = V->Val[i];

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

    return this->shiftPoint;

}

/****************************************************************************/

int FloatedBigDigit32::order() {

    return this->shiftPoint * floatedBigDigit_K;

}

/****************************************************************************/

int FloatedBigDigit32::valid_size() {

    this->shiftMax();

    int k;

    for(k=this->N+1;k>=1;k--) if(this->Val[k-1]!=0) break;

    return k;

}

/****************************************************************************/

int FloatedBigDigit32::digit(int idx) {

    if(idx>=0&&idx<=this->N+floatedBigDigit_F) return this->Val[idx];

    return 0;

}

/****************************************************************************/

int FloatedBigDigit32::value(int idx) {


    if(this->isBig()) {
        return this->digit( this->shiftPoint - idx );
    } else {
        return this->digit( idx + this->shiftPoint );
    }

}

/****************************************************************************/

bool FloatedBigDigit32::checkOver() {

    if(this->Val[0]>=floatedBigDigit_unit) this->overflow();

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

    return this->minus;

}

/****************************************************************************/

bool FloatedBigDigit32::isSmall() {

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

    if(this->shiftPoint!=0) return false;

    return !this->isSmall();

}

/****************************************************************************/

bool FloatedBigDigit32::isBig() {

    if(this->shiftPoint<=0) return false;

    return true;

}

/****************************************************************************/

bool FloatedBigDigit32::isOver() {

    if(this->shiftPoint == floatedBigDigit_unit) return true;

    return false;

}

/****************************************************************************/

bool FloatedBigDigit32::isZero() {

    for(int i=0;i<=this->N;i++) if(this->Val[i]!=0) return false;

    return true;

}

/****************************************************************************/

bool FloatedBigDigit32::isEmpty() {

    for(int i=0;i<=this->N+floatedBigDigit_F;i++) if(this->Val[i]!=0) return false;

    return true;

}

/****************************************************************************/

int FloatedBigDigit32::FD() {

    if(!this->isSmall()) return floatedBigDigitOK;

    for(int i = this->N+1;i<=this->N+floatedBigDigit_F;i++) this->Val[i] = 0;

    return floatedBigDigitOK;

}

/****************************************************************************/

int FloatedBigDigit32::FR() {

    if(!this->isSmall()) return floatedBigDigitOK;

    int CR = floatedBigDigit_unit/2;

    for(int i = this->N+1;i>=0;i--) {
        int v = this->Val[i] + CR;
        int CR = (v/floatedBigDigit_unit);
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

    this->minus = false;

    return floatedBigDigitOK;

}

/****************************************************************************/

int FloatedBigDigit32::Sig() {

    this->minus = !this->minus;

    return floatedBigDigitOK;

}

/****************************************************************************/

int FloatedBigDigit32::shift(int val) {

    return this->shiftSmall(val);

}

/****************************************************************************/

int FloatedBigDigit32::shiftSmall(int val) {

    if(val==0) return floatedBigDigitOK;
    if(this->isEmpty()) return floatedBigDigitOK;
    if(this->isOver()) return floatedBigDigitERR;

    bool rf = this->minus;

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
        int vh = v / 1000;
        int vl = v % 1000;

        vh *= val;
        vl *= val;

        vl += CR;

        vh += vl/1000;
        vl = vl % 1000;

        CR = vh / 100;
        vh = vh % 100;

        this->Val[i] = vh*1000 + vl;

    }

    if(CR>0) {
        FloatedBigDigit32* BB = new FloatedBigDigit32();
        BB->set(CR);
        BB->shiftPoint = this->shiftPoint + 1;
        this->Add(BB);
        delete BB;
    }

    this->minus = rf;

    this->isSmall();

    return floatedBigDigitOK;

}   

/****************************************************************************/
/****************************************************************************/

int FloatedBigDigit32::div(int val) {

    return this->divSmall(val);

}

/****************************************************************************/

int FloatedBigDigit32::divSmall(int val) {

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
        int v = this->Val[i] + CR*floatedBigDigit_unit;
        this->Val[i] = v/val;
        CR = v - this->Val[i]*val;
    }       

    CR = (CR*2/val>=1) ? 1: 0;

    this->shiftMax();

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

    return ret;

}   

/****************************************************************************/
/****************************************************************************/

int FloatedBigDigit32::sub(int val) {


    return this->subSmall(val);

}

/****************************************************************************/

int FloatedBigDigit32::subSmall(int val) {

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

        int GT = +1;
        int LT = -1;
        int EQ =  0;
        int RV =  1;

        if( this->isMinus() && !V->isMinus()) return LT;
        if(!this->isMinus() &&  V->isMinus()) return GT;
        if( this->isMinus() &&  V->isMinus()) RV = -1;

        int sf = V->shiftPoint - this->shiftPoint;

        if( sf >= 0 ) {

            for(int i = 0;i<=this->N+floatedBigDigit_F;i++) {

                if(this->digit(i-sf)>V->digit(i)) return GT*RV;
                if(this->digit(i-sf)<V->digit(i)) return LT*RV;

            }

        } else {

            for(int i = 0;i<=this->N+floatedBigDigit_F;i++) {

                if(this->digit(i)>V->digit(i+sf)) return GT*RV;
                if(this->digit(i)<V->digit(i+sf)) return LT*RV;

            }

        }

        return EQ;

    }

/****************************************************************************/
/****************************************************************************/

int FloatedBigDigit32::toString(char* str,int n) {


    int ptr = 0;

    if(n<this->N*(floatedBigDigit_K+1)+8) return floatedBigDigitERR;


    if (this->isMinus()) {
        str[ptr++] = '-';
    } else {
        str[ptr++] = '+';
    }

    for(int i=0;i<this->N;i++) {
        int val = this->Val[i];
        int p = floatedBigDigit_unit;
        for(int j=0;j<floatedBigDigit_K;j++) {
            if(val>=p) {
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

    if(ord>=0) {
        str[ptr++] = '+';
    } else {
        str[ptr++] = '-';
        ord = (-ord);
    }

    int p = 100000;

    for(int j=0;j<5;j++) {
        if(ord>=p) {
            str[ptr++] = '*';
        } else {
            p /= 10;
            int v = ord / p;
            str[ptr++] = '0' + v;
            ord -= v*p;
        }
    }

    str[ptr++] = '\0';

    return floatedBigDigitOK;

}

/****************************************************************************/

int FloatedBigDigit32::footString(char* str,int n) {


    int ptr = 0;

    if(n<(floatedBigDigit_K+1)*floatedBigDigit_F+1) return -1;

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

int FloatedBigDigit32::toString2(char* str,int n) {

    memset(str,'0',n);
    str[n] = '\0';

    int ptr = n - 1;

    if(this->isMinus()) return floatedBigDigitERR;
    if(this->isSmall()) return floatedBigDigitERR;

    for(int i=this->zero_pos();i>=0;i--) {

       int val = this->digit(i);

       for(int j=0;j<5;j++) {
            if(ptr<0) {
                if(val>0) {
                    memset(str,'*',n);
                    return floatedBigDigitERR;
                }
            } else {
                char p = '0' + ( val % 10 );
                str[ptr--] = p;
                val /= 10;
            }
       }

    }

    return floatedBigDigitOK;

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

    this->Copy(AA);

    if(CR>0) {
        AA->set(CR);
        AA->shiftPoint = this->shiftPoint + 1;
        this->Add(AA);
    }

    this->shiftMax();

    delete AA;

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

    this->shiftMax();

    delete AA;

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

    this->shiftMax();

    delete BB;
    delete RR;
    delete CC;

    return floatedBigDigitOK;

}

/****************************************************************************/
/****************************************************************************/

int FloatedBigDigit32::Div(FloatedBigDigit32* V) {


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

    if(V->isSeed()) {
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

    if(V->isZero()) {
        this->overflow();
        return floatedBigDigitERR;
    }

    FloatedBigDigit32* BB = new FloatedBigDigit32();

    BB->Copy(this);

    BB->Div(V);

    BB->Int();

    BB->Mul(V);

    this->Sub(BB);

    delete BB;

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

      int ret = -1;
//    int ret = this->SetAtan(C);

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

int FloatedBigDigit32::SetSin(FloatedBigDigit32* V) {

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

    if(F->isZero()) {

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
    } while(C->compare(floatedBigDigit_LMT*2) < 0 && !this->lastBit());

    bool stop = ( C->compare(floatedBigDigit_LMT*2)>=0 );

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
    } while(C->compare(floatedBigDigit_LMT*2) < 0 && !this->lastBit());

    bool stop = ( C->compare(floatedBigDigit_LMT*2)>=0 );

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

    FloatedBigDigit32* C = new FloatedBigDigit32();

    this->SetSin(V);

    C->SetCos(V);

    this->Div(C);

    delete C;

    return floatedBigDigitOK;

}

/****************************************************************************/

