#ifndef _delay_H_
#define _delay_H_
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

//生成文件名
#define FILE_NAME "Delay.txt"

//延时函数的单位(优先级:us>ms>s)
// #define us "us"
// #define ms "ms"
#define s "s"

//开启打印宏函数模式
#define PRINT_DEF
//开启检查模式
// #define CHECK
//指令的机器周期 默认NOP=1
#define DJNZ_NOT_JUMP 2
#define DJNZ_JUMP 3
#define CALL 3
#define RET 3
#define MOV 1
#define CLR 1

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

//时钟频率(MHZ)
extern double CPU;
//机器周期/时钟周期
extern int SPEED;

#if defined(us)
#define DS us
#elif defined(ms)
#define DS ms
#elif defined(s)
#define DS s
#endif

typedef long long int LL;

#define PRINT_FP(x) fprintf(fp, x)
#ifdef PRINT_DEF
#define PRINT_BRGIN "#ifdef _DELAY_%d" DS "_\n", t
#define PRINT_MAIN "_DELAY_fun_(%d, %s, %d, %d, %d, %d, %d, %d)\n", PRINT_ARGV
#define PRINT_ARGV t, DS, ind, P[0], P[1], P[2], P[3], P[4]
#define PRINT_END "#endif\n"
#else
#define PRINT_BRGIN "void Delay%d" DS "(void)\n{\n\t", t
#define PRINT_INIT "%c=%d%s", 'i' + i - 1, P[i], (i == ind) ? (";\n\t") : (",")
#define PRINT_NOP "_nop_()%s", (i == P[0]) ? (";\n\t") : (",")
#define PRINT_WHILE "}while(--%c);", 'i' + i - 1
#endif

void delay_call(FILE *, int); //调用&验证
#define delay(x) delay_call(fp, x)

#endif
