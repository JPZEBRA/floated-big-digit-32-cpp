#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FloatedBigDigit32.h"

int main(int argc,const char **argv) {

    char buff[10000];

    if(argc!=2) {
        printf("USAGE: full KETA\n");
        return 0;
    }

    long KETA = atol(argv[1]);

    SetFloatedBigDigit32Keta((int)KETA);

    FloatedBigDigit32* VAL = new FloatedBigDigit32();

    VAL->SetFull();
 
    VAL->toString2(buff,KETA);

    printf("%s\n",buff);
  
  
    delete VAL;

    FreeFloatedBigDigit32();

    return 0;

}

