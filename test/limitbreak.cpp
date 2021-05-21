#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FloatedBigDigit32.h"

int main(int argc,const char **argv) {

    char buff[10000];

    if(argc!=2) {
        printf("USAGE: limitbreak KETA\n");
        return 0;
    }

    long KETA = atol(argv[1]);


    SetFloatedBigDigit32Keta((int)KETA);

    FloatedBigDigit32* PI = new FloatedBigDigit32();

    FloatedBigDigit32* V1 = new FloatedBigDigit32();

    FloatedBigDigit32* V2 = new FloatedBigDigit32();

    FloatedBigDigit32* N = new FloatedBigDigit32();

    FloatedBigDigit32* B = new FloatedBigDigit32();

    PI->SetPI();


    printf("\n--- TEST ATAN ---\n");


    int ret;

    for(int lb=0; lb<=1;lb++) {

        B->set(1);

        for(int i=0;i<15;i++) {

            B->mul(3);

            B->div(4);

            N->set(45);

            N->Sub(B);

            double ang = N->toDouble();

            N->Mul(PI);

            N->div(180);

            V1->SetTan(N);

            ret = V2->SetAtan(V1);

            V2->mul(180);

            V2->Div(PI);

            V2->toString(buff,10000,false);

            printf("%f %s R:%d \n",ang,buff,ret);

        }

        if(lb>0) break;

        printf("\n>>> LIMIT BREAK <<<\n");

        Fbd32LimitBreak();

    }

    delete PI;
    delete V1;
    delete V2;
    delete N;
    delete B;

    FreeFloatedBigDigit32();

    return 0;

}

