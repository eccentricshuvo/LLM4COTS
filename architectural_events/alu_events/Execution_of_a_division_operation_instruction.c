int main(void)
{
    volatile int a = 12345;
    volatile int b = 123;
    volatile int c;

    /* Perform signed division to trigger op_div_i signal */
    c = a / b;

    /* Also perform signed remainder */
    c = a % b;

    return 0;
}