#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FloatedBigDigit32.h"

int main(int argc,const char **argv) {

    char buff[10000];

    if(argc!=3) {
        printf("USAGE: dfct KETA n\n");
        return 0;
    }

    long KETA = atol(argv[1]);

    long n = atol(argv[2]);

    SetFloatedBigDigit32Keta((int)KETA);

    FloatedBigDigit32* VAL = new FloatedBigDigit32();
    FloatedBigDigit32* N = new FloatedBigDigit32();

    N->set(n);

    VAL->SetDoubleFactorial(N);

    VAL->toString2(buff,(int)KETA);

    printf("%s\n",buff);
  
    delete VAL;
    delete N;

    FreeFloatedBigDigit32();

    return 0;

}

