#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FloatedBigDigit32.h"

#define cash1_max 100000
#define cash2_max 10000000

FloatedBigDigit32* cash_mem1[cash1_max];

int cash_cnt1;

char* cash_mem2;
int fketa;
int cash_cnt2;

long Index(FloatedBigDigit32* V) {

    long idx = 0;

    int ord = V->order() / 5;

    for(int i=ord;i>=0;i--) {

        if(i>=2) return -1;

        idx = idx*100000 + V->value(i);
    }

    return idx;
}

int Store(FloatedBigDigit32* V) {

    if(cash_cnt1<cash1_max) cash_mem1[cash_cnt1++]->Copy(V);

    char buff[100];
    if(cash_cnt2>=cash2_max) return -1;
    V->toString2(buff,fketa);
    memcpy(&cash_mem2[(fketa+1)*cash_cnt2++],buff,fketa+1);
    return 0;
}

int Load(int idx,FloatedBigDigit32* V) {

    if(cash_cnt1>0 && idx<cash_cnt1) {
        V->Copy(cash_mem1[idx]);
        return 0;
    }

    if(cash_cnt2==0) return -1;
    if(idx>=cash_cnt2) return -1;
    V->set(0);
    int ptr =(fketa+1)*idx + 1;
    for(int i=0;i<fketa;i++) {
        V->mul(10);
        V->add(cash_mem2[ptr++] - '0');
    }

    return 0;
}


int main(int argc,const char **argv) {

    char buff[1000];

    if(argc!=2) {
        printf("USAGE : primes KETA\n");
        return 0;
    }

    long KETA = atol(argv[1]);

    if(KETA<1) return -1;

    SetFloatedBigDigit32Keta((int)KETA);

    for(int i=0;i<cash1_max;i++) cash_mem1[i] = new FloatedBigDigit32();     
    cash_cnt1 = 0;

    fketa = KETA;
    cash_mem2 = (char*) malloc((fketa+1)*cash2_max);
    cash_cnt2 = 0;

    FloatedBigDigit32* CNT1 = new FloatedBigDigit32();
    FloatedBigDigit32* CNT2 = new FloatedBigDigit32();
    FloatedBigDigit32* CNT3 = new FloatedBigDigit32();
    FloatedBigDigit32* VAL = new FloatedBigDigit32();
    FloatedBigDigit32* MAX = new FloatedBigDigit32();

    FloatedBigDigit32* D = new FloatedBigDigit32();
    FloatedBigDigit32* P = new FloatedBigDigit32();
    FloatedBigDigit32* B = new FloatedBigDigit32();
    FloatedBigDigit32* Z = new FloatedBigDigit32();


    CNT1->set(1);
    CNT2->set(0);
    Z->set(0);

    MAX->set(1);
    for(int i=0;i<KETA;i++) MAX->mul(10);


    do {

        CNT1->add(1);


        CNT3->set(0);

        B->set(1);

        do {

            if(Load(Index(CNT3),B)<0) B->add(1);

            CNT3->add(1);

            P->Copy(CNT1);
            P->Mod(B);
            D->Copy(B);
            D->Mul(B);
            if(P->Compare(Z)==0) break;

        } while(D->Compare(CNT1)<=0);

        if(D->Compare(CNT1)>0) {
            VAL->Copy(CNT1);
            CNT2->add(1);

            Store(VAL);

            VAL->toString2(buff,KETA);
            printf("%s",buff);

        }


    } while(CNT1->Compare(MAX)<0);

    delete CNT1;
    delete CNT2;
    delete CNT3;
    delete VAL;
    delete MAX;

    delete D;
    delete P;
    delete B;
    delete Z;


    FreeFloatedBigDigit32();

    free(cash_mem2);

    return 0;

}

