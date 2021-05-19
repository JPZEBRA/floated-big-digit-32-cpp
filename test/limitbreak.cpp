#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FloatedBigDigit32.h"

int main(int argc,const char **argv) {

    char buff[10000];

    if(argc!=3) {
        printf("USAGE: limitbreak KETA n \n");
        return 0;
    }

    long KETA = atol(argv[1]);

    long n = atol(argv[2]);

    long st = n / 50;

    if(st<1) st = 1;

    if(st>=100000) return -1;


    SetFloatedBigDigit32Keta((int)KETA);

    FloatedBigDigit32* VAL = new FloatedBigDigit32();

    FloatedBigDigit32* N = new FloatedBigDigit32();

    printf("\n--- TEST COSH ---\n");

    N->set(0);

    for(long i=0;i<=n;i+=st) {

       int ret = VAL->SetCosh(N);

       if(ret!=0) break;

       VAL->toString(buff,10000,false);

       printf("+%8.1f %s R:%d\r",(float)i,buff,ret);

       N->add((int)st);
  
    }

    printf("\n--- LIMIT BREAK ---\n");

    Fbd32LimitBreak();

    N->set(0);

    for(long i=0;i<=n;i+=st) {

       int ret = VAL->SetCosh(N);

       if(ret!=0) break;

       VAL->toString(buff,10000,false);

       printf("+%8.1f %s R:%d\r",(float)i,buff,ret);

       N->add((int)st);
  
    }

    printf("\n");


    delete VAL;
    delete N;

    FreeFloatedBigDigit32();

    return 0;

}

