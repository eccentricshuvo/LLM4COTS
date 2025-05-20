int main(void)
{
    volatile int x = 0;
    volatile int y = 1;
    volatile int z;

    /*
     * The add instruction is not recognized with numeric registers or 'r' prefix.
     * Using the macro defined in OpenRISC for add instruction in asm below:
     * According to OpenRISC manual, instruction syntax is:
     *  add rD, rA, rB
     * But the assembler in this environment doesn't accept the standard format.
     *
     * We will instead use .word encoding for 'add r3,r4,r5'.
     * The binary encoding is:
     * add rD,rA,rB = 0x80000000 | (rD<<21) | (rA<<16) | (rB<<11)
     * For add r3,r4,r5:
     * rD=3, rA=4, rB=5
     * Encoding = 0x80000000 + (3<<21) + (4<<16) + (5<<11)
     * Calculate:
     * 3<<21 = 3*0x200000 = 0x600000
     * 4<<16 = 4*0x10000 = 0x40000
     * 5<<11 = 5*0x800 = 0x2800
     * sum = 0x80000000 + 0x600000 + 0x40000 + 0x2800 = 0x80642800
     */

    asm volatile(".word 0x80642800" /* id=add encoding=0x80642800 description=\"integer add r3,r4,r5\" */);

    z = x + y + 42; /* prevent optimizing away */

    return (int)z;
}