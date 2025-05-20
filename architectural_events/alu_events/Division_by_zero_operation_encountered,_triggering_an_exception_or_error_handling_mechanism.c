int main(void)
{
    volatile int divisor = 0;
    volatile int dividend = 10;
    volatile int result;

    // Use inline assembly to perform the division instruction explicitly,
    // triggering the div_by_zero event at the CPU instruction level.
    // Using OpenRISC division instruction 'div rA,rB,rC' syntax:
    // rA = dividend, rB = divisor, rC = result register
    // The divisor is zero to trigger the exception.

    __asm__ volatile (
        "l.addi r3, r0, 10\n"    // r3 = dividend
        "l.addi r4, r0, 0\n"     // r4 = divisor (zero)
        "l.div  r5, r3, r4\n"    // r5 = r3 / r4, triggers div_by_zero
    );

    return 0;
}