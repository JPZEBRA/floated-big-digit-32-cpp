#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "FloatedBigDigit32.h"

int main(int argc,const char **argv) {

    char buff[200];

    if(argc!=3) {
        printf("USAGE : seq KETA n\n");
        return 0;
    }

    long KETA = atol(argv[1]);
    long n = atol(argv[2]);

    SetFloatedBigDigit32Keta((int)KETA);

    FloatedBigDigit32* VAL = new FloatedBigDigit32();
    FloatedBigDigit32* N = new FloatedBigDigit32();
    FloatedBigDigit32* K = new FloatedBigDigit32();

    N->set((int)n);

    for(int k=0; k<=(int)n ; k++) {

        K->set(k);

        int ret = VAL->SetCombination(N,K);

        VAL->toString2(buff,80);

        printf("%4d C %4d = %s R:%d\n",(int)n,k,buff,ret);

    }

    delete VAL;
    delete N;
    delete K;

    FreeFloatedBigDigit32();

    return 0;

}

