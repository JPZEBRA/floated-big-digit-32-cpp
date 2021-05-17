#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FloatedBigDigit32.h"

int main(int argc,const char **argv) {

    char buff[10000];

    if(argc!=2) {
        printf("USAGE: atan2 KETA\n");
        return 0;
    }

    long KETA = atol(argv[1]);

    SetFloatedBigDigit32Keta((int)KETA);

    FloatedBigDigit32* PI = new FloatedBigDigit32();
    FloatedBigDigit32* VAL = new FloatedBigDigit32();
    FloatedBigDigit32* N = new FloatedBigDigit32();
    FloatedBigDigit32* S = new FloatedBigDigit32();
    FloatedBigDigit32* C = new FloatedBigDigit32();

    PI->SetPI();

    for(int i=0;i<=3600;i++) {

       N->set(i);
       N->div(10);

       N->Mul(PI);
       N->div(180);

       S->SetSin(N);
       C->SetCos(N);

       int ret = VAL->SetAtan2(S,C);

       VAL->mul(180);
       VAL->Div(PI);


       VAL->toString(buff,10000);

       printf("%5.1f %s R:%d\n",(float)i/10,buff,ret);
  
    }

    delete PI;
    delete VAL;
    delete N;
    delete S;
    delete C;

    FreeFloatedBigDigit32();

    return 0;

}

