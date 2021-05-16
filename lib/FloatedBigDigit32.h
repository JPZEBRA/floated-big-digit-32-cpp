/* FLOATED BIG DIGIT CLASS */
/* CREATE  2021.02.06      */
/* REVISED 2021.05.15      */
/* Ver 0.201  ( U.C. )     */
/* Original by K-ARAI      */


/****************************************************************************/
/****************************************************************************/

int SetRandomSeed(int seed);

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

    /****************************************************************************/

    public :

        FloatedBigDigit32();
        ~FloatedBigDigit32();

        int clear();
        int set(int val);

        int Copy(FloatedBigDigit32* V);

        int size();
        int zero_pos();
        int order();

        int valid_size();
        int digit(int idx);
        int value(int idx);

        bool checkOver();
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

        int toString(char* str,int n);
        int footString(char* str,int n);
        int toString2(char* str,int n);

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

        int SetE();

        int SetPI();

    /****************************************************************************/

        int SetSin(FloatedBigDigit32* V);
        int SetCos(FloatedBigDigit32* V);
        int SetTan(FloatedBigDigit32* V);


};