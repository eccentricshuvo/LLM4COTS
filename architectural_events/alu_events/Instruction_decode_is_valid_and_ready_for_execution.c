int main(void)
{
    volatile int a = 10, b = 20, c = 0;
    volatile int array[4] = {0x11223344, 0x55667788, 0x99AABBCC, 0xDDEEFF00};
    volatile int index = 0;

    // 1. Simple arithmetic operations
    c = a + b;
    c = b - a;

    // 2. Logical operations
    c = a & b;
    c = a | b;

    // 3. Load and store operations
    index = array[2];
    array[1] = index;

    // 4. Control flow instructions using conditional branches
    if (c > 0)
        c = b;
    else
        c = a;

    // 5. Multiply operation (may cause execution stalls)
    c = a * b;

    // 6. Insert explicit instructions with varied decode complexities
    // Use OpenRISC-1000 mnemonics from JSON:
    // l.add, l.sub, l.and, l.or, l.mul, l.lwz, l.sw, l.bf, l.nop

    asm volatile(
        "l.add r3, r4, r5\n\t"     // add
        "l.mul r6, r3, r7\n\t"     // multiply
        "l.lwz r8, 0(r9)\n\t"      // load word zero
        "l.sw r8, 4(r9)\n\t"       // store word
        "l.bf 8\n\t"               // branch forward by 8 bytes (simulate branch)
        "l.nop\n\t"                // nop as bubble
        :
        :
        : "r3", "r4", "r5", "r6", "r7", "r8", "r9"
    );

    return (int)c;
}