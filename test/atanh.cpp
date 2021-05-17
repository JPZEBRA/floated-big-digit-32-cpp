#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FloatedBigDigit32.h"

int main(int argc,const char **argv) {

    char buff[10000];

    if(argc!=3) {
        printf("USAGE: atanh KETA n \n");
        return 0;
    }

    long KETA = atol(argv[1]);

    long n = atol(argv[2]);

    SetFloatedBigDigit32Keta((int)KETA);

    FloatedBigDigit32* VAL = new FloatedBigDigit32();

    FloatedBigDigit32* V= new FloatedBigDigit32();

    FloatedBigDigit32* N = new FloatedBigDigit32();

    for(int i=0;i<=n*10;i++) {

       N->set(i);
       N->div(10);

       V->SetTanh(N);

       int ret = VAL->SetAtanh(V);

       VAL->toString(buff,10000);

       printf("+%5.1f %s R:%d\n",(float)i/10,buff,ret);
  
    }

    for(int i=0;i<=n*10;i++) {

       N->set(i);
       N->div(10);
       N->Sig();

       V->SetTanh(N);

       int ret = VAL->SetAtanh(V);

       VAL->toString(buff,10000);

       printf("-%5.1f %s R:%d\n",(float)i/10,buff,ret);
  
    }

    delete VAL;
    delete V;
    delete N;

    FreeFloatedBigDigit32();

    return 0;

}
