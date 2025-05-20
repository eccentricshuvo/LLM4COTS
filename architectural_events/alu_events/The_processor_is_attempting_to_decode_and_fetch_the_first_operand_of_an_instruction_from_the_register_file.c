int main(void)
{
    register int r3 asm ("r3") = 5;
    register int r4 asm ("r4") = 10;
    register int r5 asm ("r5");

    // Correct encoding for: add r5, r3, r4
    // Opcode base 0x0c0000
    // rD=5, rA=3, rB=4
    // Encoding = 0x0c0000 | (5 << 8) | (3 << 4) | 4 = 0x0c0000 + 0x0500 + 0x30 + 0x04 = 0x0c0534

    asm volatile(".word 0x0c0534"); // add r5, r3, r4

    return r5;
}