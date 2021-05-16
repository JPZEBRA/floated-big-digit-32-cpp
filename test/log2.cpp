#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FloatedBigDigit32.h"

int main(int argc,const char **argv) {

    char buff[10000];

    if(argc!=3) {
        printf("USAGE: exp KETA n \n");
        return 0;
    }

    long KETA = atol(argv[1]);

    long n = atol(argv[2]);

    SetFloatedBigDigit32Keta((int)KETA);

    FloatedBigDigit32* VAL = new FloatedBigDigit32();

    FloatedBigDigit32* N = new FloatedBigDigit32();

    for(int i=1;i<=n*10;i++) {

       N->set(i);
       N->div(10);

       VAL->SetLog2(N);

       VAL->toString(buff,10000);

       printf("ln +%5.1f %s\n",(float)i/10,buff);
  
    }

    delete VAL;
    delete N;

    FreeFloatedBigDigit32();

    return 0;

}

