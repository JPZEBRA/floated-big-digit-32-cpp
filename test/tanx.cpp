#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FloatedBigDigit32.h"

int main(int argc,const char **argv) {

    char buff[10000];

    if(argc!=2) {
        printf("USAGE: tanx KETA\n");
        return 0;
    }

    long KETA = atol(argv[1]);

    SetFloatedBigDigit32Keta((int)KETA);

    FloatedBigDigit32* PI = new FloatedBigDigit32();
    FloatedBigDigit32* VAL = new FloatedBigDigit32();
    FloatedBigDigit32* N = new FloatedBigDigit32();

    PI->SetPI();

    for(int i=0;i<=3600;i++) {

       N->set(i);
       N->div(10);

       N->Mul(PI);
       N->div(180);

       VAL->SetTan(N);

       VAL->toString(buff,10000);

       printf("%5.1f %s\n",(float)i/10,buff);
  
    }

    delete PI;
    delete VAL;
    delete N;

    FreeFloatedBigDigit32();

    return 0;

}

