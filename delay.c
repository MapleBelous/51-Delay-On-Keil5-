#include "delay.h"

#define Maxind 5 //循环变量的最大值，超出立即引发程序错误

double CPU;
int SPEED;

bool iskey_init = false; //是否已初始化最大值
LL ks[Maxind];           //剩余量缓存
LL key[Maxind];          //最大值
int P[Maxind];           //ijk值记录
int ind;                 //循环变量个数

void delay_init(void);         //首次运行时，初始化最大值数组
bool delay_us(LL k);           //数据初始化，调整nop值
void delay_re(int);            //初次生成初始值+nop
void delay_print(FILE *, int); //打印生成函数，CHECK宏开启时，关闭输出。
bool delay_check(LL);          //CHECK宏开启时，检验延时函数正确性，错误的生成将报错。（已校验，无错误）

void delay_call(FILE *fp, int t)
{
    if (!iskey_init)
        delay_init(), iskey_init = true;
#if defined(us)
    LL x = t;
#elif defined(ms)
    LL x = t * 1000LL;
#elif defined(s)
    LL x = t * 1000000LL;
#endif

    x = round(x * CPU / SPEED); //取最近周期数
    int pr = 0;
    while (delay_us(x - pr)) //免除有i/j/k等于0且nop无法补偿的情况
        ++pr;
    P[0] += pr; //用nop补偿
#ifdef CHECK
    delay_check(x);
#else
    delay_print(fp, t); //打印函数
#endif
}
void delay_init(void) //初始化最大值
{
    for (int i = 1; i != Maxind; ++i)
        key[i] = (key[i - 1] + DJNZ_NOT_JUMP) + (key[i - 1] + DJNZ_JUMP) * 255;
}
bool delay_us(LL k)
{
    LL k1 = k - CALL - RET; //CALL&RET
    if (k1 < 0)
    {
        fprintf(stderr, "error:do not need function\n");
        exit(1); //少于编写延时函数最少的机器周期
    }
    //--------------------------------------------//
    for (int i = 0; i != Maxind; ++i)
        P[i] = 0; //初始化P
    ind = 0;      //初始化ind
    //--------------------------------------------//
    if (!k1)
        return false; //刚好CALL+RET构成
    //--------------------------------------------//
    while (k1 - MOV >= key[ind] + DJNZ_NOT_JUMP && ind != Maxind)
        k1 -= MOV, ++ind; //找寻至少需要多少个循环变量
    if (ind == Maxind)
    {
        fprintf(stderr, "error:too more loop\n\n");
        exit(1); //超出允许的最大循环变量数！
    }
    //--------------------------------------------//
    ks[ind] = k1; //初始化ks
    //--------------------------------------------//
    int jnd = ind;
    if (delay_re(jnd), P[0] < 0) //是否有不合理nop值
    {
        jnd = 1;
        while (P[jnd] == 1)
            ++jnd;                           //回溯寻找首个不是1的位置
        P[jnd] = (P[jnd] + 255) % 256;       //当前位置值减一，不能直接减一，要考虑(0-1) = 255
        P[0] = (int)ks[jnd - 1] + DJNZ_JUMP; //nop补上缺少的值
        for (int i = 1; i < jnd; ++i)
            P[i] = 0; //全填入0，满足最大值
    }
    //--------------------------------------------//
    bool re = false;
    for (int i = 1; i <= ind; ++i)
        if (P[i] == 0)
            re = true;     //发现等于0的变量,keil编译时0会用CLR A赋值，导致多一个nop时间
    if (re && P[0] >= CLR) //nop不为0则可以用nop补偿
        P[0] -= CLR, re = false;
    return re;
}
void delay_re(int t)
{
    for (int i = t; i >= 1; --i)
    {
        for (int x = 1; x <= 256; ++x)
        {
            LL ans = DJNZ_NOT_JUMP;
            if (x >= 2)
                ans += (x - 1) * (key[i - 1] + DJNZ_JUMP);
            if (ks[i] - ans < key[i - 1] + DJNZ_NOT_JUMP + MOV)
            {
                ks[i - 1] = ks[i] - ans;
                P[i] = x % 256;
                break;
            }
        }
    }
    P[0] = (int)ks[0]; //转移剩余的周期作为NOP
}
void delay_print(FILE *fp, int t)
{
#ifdef PRINT_DEF
    PRINT_FP(PRINT_BRGIN);
    PRINT_FP(PRINT_MAIN);
    PRINT_FP(PRINT_END);
#else
    PRINT_FP(PRINT_BRGIN);
    PRINT_FP("unsigned char ");
    for (int i = 1; i <= ind; ++i)
        PRINT_FP(PRINT_INIT);
    for (int i = 1; i <= P[0]; ++i)
        PRINT_FP(PRINT_NOP);
    for (int i = 2; i <= ind; ++i)
        PRINT_FP("do{");
    if (ind >= 1)
        PRINT_FP("while(--i);");
    for (int i = 2; i <= ind; ++i)
        PRINT_FP(PRINT_WHILE);
    if (ind >= 1)
        PRINT_FP("\n");
    PRINT_FP("}\n");
#endif
}
bool delay_check(LL x)
{
    LL ans = CALL + RET + MOV * ind + P[0]; //CALL+RET+MOV+NOP
    bool iszero = false;
    for (int i = 1; i <= ind; ++i)
    {
        ans += DJNZ_NOT_JUMP;
        if (!P[i] && iszero == false)
            ++ans, iszero = true; //CLR
        int Pt = (P[i] + 255) % 256;
        if (Pt)
            ans += (Pt) * (key[i - 1] + DJNZ_JUMP);
    }
    bool re = x != ans;
    if (re)
    {
        printf("delay_check Warning:ans = %lld,x = %lld\n", ans, x);
        printf("ind = %d,nop = %d\n", ind, P[0]);
        printf("P[1] = %d,P[2] = %d,P[3] = %d\n", P[1], P[2], P[3]);
    }
    return re;
}
