#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FloatedBigDigit32.h"

int main(int argc,const char **argv) {

    char buff[10000];

    if(argc!=3) {
        printf("USAGE: loge KETA value \n");
        return 0;
    }

    long KETA = atol(argv[1]);

    SetFloatedBigDigit32Keta((int)KETA);

    FloatedBigDigit32* VAL = new FloatedBigDigit32();
    FloatedBigDigit32* N = new FloatedBigDigit32();

    N->Set(argv[2]);

    int ret = VAL->SetLn(N);

    VAL->toString(buff,10000,false);

    printf("%s R:%d\n",buff,ret);
  
    delete VAL;
    delete N;

    FreeFloatedBigDigit32();

    return 0;

}

