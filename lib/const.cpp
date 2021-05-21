#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FloatedBigDigit32.h"

int main(int argc,const char **argv) {

    char buff[20000];

    int ret;

    if(argc!=2) {
        printf("USAGE: const KETA\n");
        return 0;
    }

    long KETA = atol(argv[1]);

    SetFloatedBigDigit32Keta((int)KETA);

    FloatedBigDigit32* VAL = new FloatedBigDigit32();

    ret = VAL->SetE();

    VAL->toString(buff,20000);

    printf("%s R:%d\n",buff,ret);

    ret = VAL->SetPI();

    VAL->toString(buff,20000);

    printf("%s R:%d\n",buff,ret);

    delete VAL;

    FreeFloatedBigDigit32();

    return 0;

}

