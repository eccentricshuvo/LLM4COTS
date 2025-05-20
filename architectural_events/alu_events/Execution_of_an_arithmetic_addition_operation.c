int main(void)
{
    volatile int a = 1;
    volatile int b = 2;
    volatile int c = 0;

    /* Perform repeated addition operations to trigger op_add_i signal */
    for (int i = 0; i < 1000; ++i)
    {
        c = a + b;
        a = b + c;
        b = a + c;
    }

    /* Return the accumulated value to prevent optimization */
    return c;
}