int main(void)
{
    volatile int a = 10;
    volatile int b = 5;
    volatile int c, d, e;

    /*
     * Use multiple dependent multiply and divide instructions with .word encoding.
     * This causes pipeline stalls waiting for the multiply and divide units to free up.
     * Chain multiplies and divides to increase stall likelihood.
     */

    register int r4 asm("r4") = a;
    register int r5 asm("r5") = b;
    register int r7 asm("r7") = 1; /* divisor for division */

    asm volatile(
        /* mul r3, r4, r5 */
        ".word 0x4C842805\n"
        /* mul r8, r3, r5 (dependent multiply on previous result) */
        ".word 0x4C883805\n"
        /* div r6, r8, r7 */
        ".word 0x48063307\n"
        /* div r9, r6, r7 (dependent divide) */
        ".word 0x48067407\n"
        : "=r"(c), "=r"(d)
        : "r"(r4), "r"(r5), "r"(r7)
        : "r3", "r6", "r8", "r9"
    );

    e = c + d;

    return 0;
}