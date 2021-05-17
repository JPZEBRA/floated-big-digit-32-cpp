#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FloatedBigDigit32.h"

int main(int argc,const char **argv) {

    char buff[10000];

    if(argc!=3) {
        printf("USAGE: load KETA value\n");
        return 0;
    }

    long KETA = atol(argv[1]);

    SetFloatedBigDigit32Keta((int)KETA);

    FloatedBigDigit32* VAL = new FloatedBigDigit32();
  
    VAL->Set(argv[2]);

    VAL->toString(buff,10000);

    printf("%s\n",buff);
  
    delete VAL;

    FreeFloatedBigDigit32();

    return 0;

}

