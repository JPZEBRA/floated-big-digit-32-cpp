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

    FloatedBigDigit32* COPY = new FloatedBigDigit32();
  
    VAL->Set(argv[2]);




    VAL->toString(buff,10000);

    printf("[RAW DATA]\n");

    printf("%s\n",buff);

    COPY->LoadString(buff);

    COPY->toString(buff,10000);

    printf("[COPY]\n");

    printf("%s\n",buff);




    VAL->toString(buff,10000,false);

    printf("[SHIFT DATA]\n");

    printf("%s\n",buff);

    COPY->LoadString(buff);

    COPY->toString(buff,10000);

    printf("[COPY]\n");

    printf("%s\n",buff);




    printf("[TOSTRING 2]\n");

    VAL->toString2(buff,50);

    printf("%s\n",buff);

    VAL->toString2(buff,50,10);

    printf("%s\n",buff);




    printf("[SEPARATE]\n");

    VAL->toSeparate(buff,5,20);

    printf("%s\n",buff);

    VAL->toSeparate(buff,4,16);

    printf("%s\n",buff);

    VAL->toSeparate(buff,3,12);

    printf("%s\n",buff);

    VAL->toSeparate(buff,3,12,9);

    printf("%s\n",buff);




    printf("[DOUBLE]\n");

    double val = VAL->toDouble();

    printf("%f\n",val);


  
    delete VAL;

    delete COPY;

    FreeFloatedBigDigit32();

    return 0;

}

