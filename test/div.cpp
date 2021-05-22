#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FloatedBigDigit32.h"

int main(int argc,const char **argv) {

    char buff[10000];

    if(argc!=4) {
        printf("USAGE: div KETA val1 val2 \n");
        return 0;
    }

    long KETA = atol(argv[1]);

    SetFloatedBigDigit32Keta((int)KETA);

    FloatedBigDigit32* VAL = new FloatedBigDigit32();

    FloatedBigDigit32* A = new FloatedBigDigit32();

    FloatedBigDigit32* B = new FloatedBigDigit32();

    A->Set(argv[2]);

    B->Set(argv[3]);

    VAL->Copy(A);

    VAL->Div(B);

    VAL->FR();
 
    VAL->toString(buff,10000);

    printf("%s\n",buff);
  
  
    delete VAL;
    delete A;
    delete B;

    FreeFloatedBigDigit32();

    return 0;

}

