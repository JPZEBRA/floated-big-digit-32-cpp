#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "FloatedBigDigit32.h"

int ValStr(char* buffer,int keta,FloatedBigDigit32* V) {

    memset(buffer,'0',keta);

    int ptr = keta - 1;

    for(int i=V->zero_pos();i>=0;i--) {

       int val = V->digit(i);

       for(int j=0;j<5;j++) {
            char p = '0' + ( val % 10 );
            buffer[ptr--] = p;
            val /= 10;
       }

    }

    buffer[keta+1] = '\0';

    return 0;
}

int main(int argc,const char **argv) {

    char buff[200];

    if(argc!=4) {
        printf("USAGE : dice K(個) N(面体) T(回)\n");
        return 0;
    }

    long KETA = 10;

    long k = atol(argv[1]);
    long n = atol(argv[2]);
    long t = atol(argv[3]);

    SetFloatedBigDigit32Keta((int)KETA);

    SetRandomSeed(time(NULL));


    FloatedBigDigit32* DICE = new FloatedBigDigit32();
    FloatedBigDigit32* K = new FloatedBigDigit32();
    FloatedBigDigit32* N = new FloatedBigDigit32();

    K->set((int)k);
    N->set((int)n);

    for(int i=0; i<t ; i++) {

        DICE->Dice(K,N);

        DICE->toString2(buff,10);

        printf("%s\n",buff);

    }

    delete DICE;
    delete K;
    delete N;

    FreeFloatedBigDigit32();

    return 0;

}

