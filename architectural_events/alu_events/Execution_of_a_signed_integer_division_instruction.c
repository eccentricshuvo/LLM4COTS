int main(void)
{
    volatile int a = -100;
    volatile int b = -3;
    volatile int c;
    /* Signed division operation - triggers op_div_signed_i event */
    c = a / b;
    /* Use result to prevent optimization */
    return c;
}