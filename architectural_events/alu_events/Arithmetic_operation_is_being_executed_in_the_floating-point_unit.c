int main(void)
{
    volatile float a = 1.23f;
    volatile float b = 4.56f;
    volatile float c = 0.0f;
    volatile int i;  /* Ensure i is volatile to avoid optimization issues */

    /* Loop to repeatedly trigger floating-point arithmetic in FPU */
    for (i = 0; i < 1000; i++) {
        c = a + b;
        c = c - a;
        c = c * b;
        c = c / a;

        /* Modify a and b to avoid compiler optimization */
        a += 0.001f;
        b -= 0.001f;
    }

    /* Return a scaled integer to use c and avoid optimization */
    return (int)(c * 1000.0f);
}