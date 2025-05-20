int main(void)
{
    volatile int a = 42;      // volatile to prevent optimization
    volatile int b = 7;
    volatile int c;

    // Force actual load and store operations on memory addresses (not just registers)
    volatile int *ptr_a = &a;
    volatile int *ptr_b = &b;
    volatile int *ptr_c = &c;

    // Load operation from pointer to trigger data cache enable signal dc_enable_i
    c = *ptr_a;

    // Store operation to pointer to trigger data cache enable signal dc_enable_i
    *ptr_b = c;

    // Arithmetic operation modifies processor state, does not trigger caching
    c = a + b;

    // Force a division by zero exception-generating instruction via inline asm
    // using exact OpenRISC 1000 mnemonics from ISA JSON file: l.movhi, l.addi, l.div
    // Dividing by zero should generate exception and influence dc_enable_i

    asm volatile (
        "l.movhi r3, 0\n\t"       /* r3 = 0 */
        "l.addi  r3, r3, 42\n\t"  /* r3 = 42 dividend */
        "l.movhi r4, 0\n\t"       /* r4 = 0 */
        "l.addi  r4, r4, 0\n\t"   /* r4 = 0 divisor */
        "l.div   r5, r3, r4\n\t"  /* r5 = r3 / r4, division by zero exception */
        :
        :
        : "r3", "r4", "r5"
    );

    return 0;
}