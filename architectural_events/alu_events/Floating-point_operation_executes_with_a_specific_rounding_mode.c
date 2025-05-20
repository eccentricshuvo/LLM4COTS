int main(void)
{
    volatile float a = 1.0f;
    volatile float b = 3.0f;
    volatile float c;

    unsigned int rmode;

    /* Set rounding mode to Round to nearest (00) and perform operation */
    rmode = 0;
    __asm__ volatile (
        "frnd %0\n\t"
        "fdiv.s %1, %2, %3\n\t"
        : /* outputs */ "+r" (rmode), "=f" (c)
        : /* inputs */ "f" (a), "f" (b)
    );

    /* Set rounding mode to Round down (01) and perform operation */
    rmode = 1;
    __asm__ volatile (
        "frnd %0\n\t"
        "fdiv.s %1, %2, %3\n\t"
        : "+r" (rmode), "=f" (c)
        : "f" (a), "f" (b)
    );

    /* Set rounding mode to Round up (10) and perform operation */
    rmode = 2;
    __asm__ volatile (
        "frnd %0\n\t"
        "fdiv.s %1, %2, %3\n\t"
        : "+r" (rmode), "=f" (c)
        : "f" (a), "f" (b)
    );

    /* Set rounding mode to Round toward zero (11) and perform operation */
    rmode = 3;
    __asm__ volatile (
        "frnd %0\n\t"
        "fdiv.s %1, %2, %3\n\t"
        : "+r" (rmode), "=f" (c)
        : "f" (a), "f" (b)
    );

    return (int)c;
}