int main(void)
{
    unsigned int a = 0xFFFFFFFFu; // Large unsigned operand
    unsigned int b = 0x2u;        // Multiplier > 1 to force overflow
    unsigned int c;

    // OpenRISC inline assembly MULU instruction usage syntax from header:
    // Syntax: mulu rd, ra, rb
    // We use registers mapped automatically by GCC on OpenRISC.
    // Constraints: "=&r"(c) for output, "r"(a), "r"(b) for inputs

    __asm__ volatile (
        "mulu %0, %1, %2\n\t"   // MULU rd, ra, rb   id=0x70 encoding
        : "=&r"(c)
        : "r"(a), "r"(b)
    );

    // Use volatile to prevent compiler optimizations removing the calculation.
    volatile unsigned int result = c;

    return (int)result;
}