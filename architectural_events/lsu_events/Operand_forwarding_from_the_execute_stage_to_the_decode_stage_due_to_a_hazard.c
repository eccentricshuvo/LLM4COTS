int main(void)
{
    /*
     * Correct mnemonics from OpenRISC-1000 ISA JSON:
     * Load word is 'l.lwz'
     * Add is 'l.add'
     * 
     * Must use prefix 'l.' for all instructions.
     * Also, commas have no spaces after them.
     */

    volatile int array[2] = {0x12345678, 0x9ABCDEF0};
    volatile int result = 0;

    register int *r4 asm("r4") = (int *)array;
    register int r6 asm("r6") = 1;

    asm volatile(
        "l.lwz r3,0(r4)\n"
        "l.add r5,r3,r6\n"
        : /* no outputs */
        : "r"(r4), "r"(r6)
        : "r3", "r5"
    );

    (void)result;
    return 0;
}