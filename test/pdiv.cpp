#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FloatedBigDigit32.h"

int main(int argc,const char **argv) {

    char buff[10000];

    if(argc!=3) {
        printf("USAGE: pdiv KETA n \n");
        return 0;
    }

    long KETA = atol(argv[1]);

    long n = atol(argv[2]);

    if(n>=100000) return -1;

    SetFloatedBigDigit32Keta((int)KETA);

    FloatedBigDigit32* VAL = new FloatedBigDigit32();

    FloatedBigDigit32* N = new FloatedBigDigit32();

    FloatedBigDigit32* D = new FloatedBigDigit32();

    N->set((int)n);

    for(int i=2;i<100;i++) {

        VAL->Copy(N);

        D->set(i);

        VAL->PowerDiv(D);
 
        VAL->toString(buff,10000);

        printf("%2dR %d %s\n",i,(int)n,buff);
  
    }
  
    delete VAL;
    delete N;
    delete D;

    FreeFloatedBigDigit32();

    return 0;

}

