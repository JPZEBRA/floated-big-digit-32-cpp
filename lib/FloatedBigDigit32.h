/* FLOATED BIG DIGIT CLASS */
/* CREATE  2021.02.06      */
/* REVISED 2021.05.17      */
/* Ver 0.6.9               */
/* Original by K-ARAI      */


/****************************************************************************/
/****************************************************************************/

#define floatedBigDigitERR -1
#define floatedBigDigitOK 0

/****************************************************************************/


int Fbd32SetRandomSeed(int seed);

int Fbd32LimitBreak();

int SetFloatedBigDigit32Keta(int k);

int FreeFloatedBigDigit32();


/****************************************************************************/

class FloatedBigDigit32 {


    /****************************************************************************/

    private :

        int N;

        int *Val;

        int shiftPoint;

        bool small;

        bool minus;

        int set_mem();


    /****************************************************************************/

    protected :

        bool lastBit();
        bool overflow();

        bool checkOver();

        int shift(int val);
        int shiftSmall(int val);
        int shiftMax();

        int mulSmall(int val);
        int divSmall(int val);
        int addSmall(int val);
        int subSmall(int val);

        int AddSmall(FloatedBigDigit32* V);
        int SubSmall(FloatedBigDigit32* V);
        int MulSmall(FloatedBigDigit32* V);
        int DivSmall(FloatedBigDigit32* V);


        int setAtan2(int y,int x);
        int setAtanDiv(int d);

        int Power_main(FloatedBigDigit32* V,bool boost);
        int PowerDiv_boost(FloatedBigDigit32* V);


    /****************************************************************************/

    public :

        FloatedBigDigit32();
        ~FloatedBigDigit32();

        int clear();
        int set(int val);

        int Set(const char* str);
        int Copy(FloatedBigDigit32* V);

        int size();
        int valid_size();
        int order();

        int zero_pos();
        int digit(int idx);
        int value(int idx);

        bool isMinus();
        bool isSmall();
        bool isSeed();
        bool isBig();
        bool isOver();
        bool isZero();
        bool isEmpty();

        int FD();
        int FR();

        int Fix();
        int Round();
        int Int();

        int Abs();
        int Sig();

    /****************************************************************************/
    /****************************************************************************/

        int mul(int val);
        int div(int val);
        int add(int val);
        int sub(int val);

    /****************************************************************************/

        int Add(FloatedBigDigit32* V);
        int Sub(FloatedBigDigit32* V);
        int Mul(FloatedBigDigit32* V);
        int Div(FloatedBigDigit32* V);

     /****************************************************************************/

        int compare(int val);
        int Compare(FloatedBigDigit32* V);

    /****************************************************************************/

        int toString(char* str,int n,bool rawdata = true);
        int footString(char* str,int n);
        int toString2(char* str,int keta,int small = 0);

    /****************************************************************************/

        int mod(int val);
        int Mod(FloatedBigDigit32* V);

    /****************************************************************************/

        int SetRnd();

        int dice1(int n);
        int dice(int a,int b);

        int Dice(FloatedBigDigit32* A,FloatedBigDigit32* B);

    /****************************************************************************/

        int setFactorial(int n);
        int SetFactorial(FloatedBigDigit32* V);

        int setDoubleFactorial(int n);
        int SetDoubleFactorial(FloatedBigDigit32* V);

    /****************************************************************************/

        int setSequence(int n,int k);
        int SetSequence(FloatedBigDigit32* N,FloatedBigDigit32* K);

        int setCombination(int n,int k);
        int SetCombination(FloatedBigDigit32* N,FloatedBigDigit32* K);


    /****************************************************************************/

        int SetE();

        int SetPI();

        int SetExp(FloatedBigDigit32* V);

        int SetLn(FloatedBigDigit32* V);
        int SetLog(FloatedBigDigit32* V);
        int SetLog2(FloatedBigDigit32* V);

    /****************************************************************************/


        int power(int n);

        int Power(FloatedBigDigit32* V);

        int PowerDiv(FloatedBigDigit32* V);

    /****************************************************************************/

        int SetSin(FloatedBigDigit32* V);
        int SetCos(FloatedBigDigit32* V);
        int SetTan(FloatedBigDigit32* V);

    /****************************************************************************/

        int SetSinh(FloatedBigDigit32* V);
        int SetCosh(FloatedBigDigit32* V);
        int SetTanh(FloatedBigDigit32* V);

    /****************************************************************************/

        int SetAtan2(FloatedBigDigit32* Y, FloatedBigDigit32* X);
        int SetAtan(FloatedBigDigit32* V);

        int SetAsin(FloatedBigDigit32* V);
        int SetAcos(FloatedBigDigit32* V);

    /****************************************************************************/

        int SetAsinh(FloatedBigDigit32* V);
        int SetAsinhB(FloatedBigDigit32* V);
        int SetAcosh(FloatedBigDigit32* V);
        int SetAtanh(FloatedBigDigit32* V);

    /****************************************************************************/

};
