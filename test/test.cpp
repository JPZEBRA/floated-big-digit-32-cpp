#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FloatedBigDigit32.h"

int main(int argc,const char **argv) {

    char buff[1000];

    if(argc!=2) {
        printf("USAGE : test KETA\n");
        return 0;
    }

    long KETA = atol(argv[1]);

    if(KETA<3) KETA = 3;

    SetFloatedBigDigit32Keta((int)KETA);

    FloatedBigDigit32* CNT1 = new FloatedBigDigit32();
    FloatedBigDigit32* MAX = new FloatedBigDigit32();

    MAX->set(1);
    for(int i=0;i<KETA;i++) MAX->mul(10);

    CNT1->set(0);

    do {

       CNT1->toString2(buff,KETA);

       printf("%s\n",buff);

       CNT1->add(1);

    } while(CNT1->Compare(MAX)<0);

    delete CNT1;
    delete MAX;

    FreeFloatedBigDigit32();

    return 0;

}

