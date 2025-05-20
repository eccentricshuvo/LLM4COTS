int main(void)
{
    volatile int base = 1000;
    volatile int offset1 = 4;
    volatile int offset2 = 8;
    volatile int result1;
    volatile int result2;

    /* The following two load-like operations with address calculations
       will trigger ctrl_lsu_adr_i by causing the hardware to compute
       base+offset addresses for memory access. Using volatile prevents optimization. */
    result1 = *((volatile int *)((char *)&base + offset1));
    result2 = *((volatile int *)((char *)&result1 + offset2));

    /* Use results to prevent optimization away */
    return (int)(result1 + result2);
}