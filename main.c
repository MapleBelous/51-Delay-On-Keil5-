#define _CRT_SECURE_NO_WARNINGS
#include "delay.h"

int main(void)
{
    FILE *fp = NULL;
#ifndef CHECK
    fp = fopen(FILE_NAME, "w");
    int a, b;
    printf("CPU=");
    scanf("%lf", &CPU);
    printf("SPEED=");
    scanf("%d", &SPEED);
    printf("a=");
    scanf("%d", &a);
    printf("b=");
    scanf("%d", &b);
    for (int i = a; i <= b; ++i)
        delay(i);
    printf("ok,Filename : " FILE_NAME);
    fclose(fp);
#elif defined(us)
    CPU = 6.0, SPEED = 1;
    for (int i = 1; i <= 10000000; ++i)
        //检查1us~10s
        delay(i);
#endif
    return 0;
}
