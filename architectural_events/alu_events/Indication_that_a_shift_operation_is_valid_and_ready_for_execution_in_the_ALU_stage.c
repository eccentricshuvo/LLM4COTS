int main(void)
{
    volatile unsigned int x = 0x12345678u;
    volatile unsigned int y;

    /* Execute various shift instructions repeatedly for stronger event triggering */
    for (int i = 0; i < 100; i++) {
        y = x << (i & 31);    /* logical shift left by varying amounts */
        y = x >> (i & 31);    /* logical shift right */

        volatile int sx = -123456789;
        volatile int sy = sx >> (i & 31);  /* arithmetic shift right */

        /* Use result to prevent optimization */
        x = y ^ (unsigned int)sy;
    }

    (void)y;

    return 0;
}