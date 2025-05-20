int main(void)
{
    volatile float a = 1.5f;
    volatile float b = 2.5f;
    volatile float c;

    /* Floating-point addition triggers op_fpu_i */
    c = a + b;

    /* Prevent optimization away */
    return (int)c;
}