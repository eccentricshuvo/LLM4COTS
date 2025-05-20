int main(void)
{
    volatile float a = 1.0f;
    volatile float b = 2.0f;
    volatile int c;

    /* Use volatile float variables to hold intermediate results.
       Use direct inline asm for fcmp instruction if available to force use of FPU comparison. */

    /* Perform floating-point comparisons with inline asm forcing fcmp usage */

    /* Compare a < b */
    __asm__ volatile (
        "fcmp %0, %1\n"
        : 
        : "f"(a), "f"(b)
        : /* no clobbers */
    );

    /* Compare a > b */
    __asm__ volatile (
        "fcmp %1, %0\n"
        : 
        : "f"(a), "f"(b)
        : /* no clobbers */
    );

    /* Compare a == b */
    __asm__ volatile (
        "fcmpe %0, %1\n"
        : 
        : "f"(a), "f"(b)
        : /* no clobbers */
    );

    /* Since the inline asm doesn't produce outputs used in C,
       set c to a fixed value to terminate cleanly */
    c = 0;

    return c;
}