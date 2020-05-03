typedef unsigned char uchar;
#define _DELAY_fun_(sec, mode, ind, nop, x, y, z, p) \
    void Delay##sec##mode##(void)                    \
    {                                                \
        _DELAY_##ind##_(x, y, z, p);                 \
        _DELAY_NOP##nop##_;                          \
        _DELAY_WHILE##ind##_;                        \
    }
#define _DELAY_NOP0_
#define _DELAY_NOP1_ _nop_()
#define _DELAY_NOP2_ _nop_(), _nop_()
#define _DELAY_NOP3_ _nop_(), _nop_(), _nop_()
#define _DELAY_NOP4_ _nop_(), _nop_(), _nop_(), _nop_()
#define _DELAY_NOP5_ _nop_(), _nop_(), _nop_(), _nop_(), _nop_()
#define _DELAY_NOP6_ _nop_(), _nop_(), _nop_(), _nop_(), _nop_(), _nop_()
#define _DELAY_0_(x, y, z, p)  
#define _DELAY_1_(x, y, z, p) uchar i = x
#define _DELAY_2_(x, y, z, p) uchar i = x, j = y
#define _DELAY_3_(x, y, z, p) uchar i = x, j = y, k = z
#define _DELAY_4_(x, y, z, p) uchar i = x, j = y, k = z, l = p
#define _DELAY_WHILE0_ 
#define _DELAY_WHILE1_ while (--i)
#define _DELAY_WHILE2_ \
    do                 \
    {                  \
        while (--i)    \
            continue;  \
    } while (--j)
#define _DELAY_WHILE3_    \
    do                    \
    {                     \
        do                \
        {                 \
            while (--i)   \
                continue; \
        } while (--j);    \
    } while (--k)
#define _DELAY_WHILE4_        \
    do                        \
    {                         \
        do                    \
        {                     \
            do                \
            {                 \
                while (--i)   \
                    continue; \
            } while (--j);    \
        } while (--k);        \
    } while (--l)

#define _DELAY_10000000us_
#ifdef _DELAY_10000000us_
void Delay10000000us(void)
{
    uchar i = 31, j = 134, k = 194, l = 5;
    ;
    do
    {
        do
        {
            do
            {
                while (--i)
                    continue;
            } while (--j);
        } while (--k);
    } while (--l);
}
#endif
