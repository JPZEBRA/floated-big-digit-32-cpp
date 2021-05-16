#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FloatedBigDigit32.h"

int main(int argc,const char **argv) {

    char buff[10000];

    if(argc!=4) {
        printf("USAGE: power KETA a b \n");
        return 0;
    }

    long KETA = atol(argv[1]);

    long a = atol(argv[2]);

    long b = atol(argv[3]);

    SetFloatedBigDigit32Keta((int)KETA);

    FloatedBigDigit32* VAL = new FloatedBigDigit32();

    FloatedBigDigit32* A = new FloatedBigDigit32();

    FloatedBigDigit32* B = new FloatedBigDigit32();

    A->set((int)a);

    B->set((int)b);

    VAL->Copy(A);

    VAL->Power(B);
 
    VAL->toString(buff,10000);

    printf("%s\n",buff);
  
  
    delete VAL;
    delete A;
    delete B;

    FreeFloatedBigDigit32();

    return 0;

}

