#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FloatedBigDigit32.h"

int main(int argc,const char **argv) {

    char buff[10000];

    if(argc!=2) {
        printf("USAGE: asin KETA\n");
        return 0;
    }

    long KETA = atol(argv[1]);

    SetFloatedBigDigit32Keta((int)KETA);

    FloatedBigDigit32* PI = new FloatedBigDigit32();
    FloatedBigDigit32* VAL = new FloatedBigDigit32();
    FloatedBigDigit32* N = new FloatedBigDigit32();
    FloatedBigDigit32* V = new FloatedBigDigit32();

    PI->SetPI();

    for(int i=0;i<=9000;i+=5) {

       N->set(i);
       N->div(100);

       N->Mul(PI);
       N->div(180);

       V->SetSin(N);

       int ret = VAL->SetAsin(V);

       VAL->mul(180);
       VAL->Div(PI);

       VAL->toString(buff,10000);

       printf("%5.2f %s R:%d\n",(float)i/100,buff,ret);
  
    }

    delete PI;
    delete VAL;
    delete N;
    delete V;

    FreeFloatedBigDigit32();

    return 0;

}

