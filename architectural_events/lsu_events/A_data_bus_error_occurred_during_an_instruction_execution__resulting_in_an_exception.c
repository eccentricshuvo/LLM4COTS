int main(void)
{
    int val;
    /* The OpenRISC-1000 ISA uses 'lwz' with registers r0-r31 but
       the assembler expects registers to be written as r0..r31.

       However, the constraints "%0" in inline asm pick any register,
       possibly out of range of r0-r31 or a non-GPR. The error suggests
       that conversion to r17 (the register used) is not accepted in lwz.

       To work around the assembler rejection, use ".word" directive
       with the exact encoding of lwz instruction manually.

       Encoding lwz is:
       op[31:26] = 0b100000 = 0x20 (opcode)
       rD[25:21] = dest reg
       rA[20:16] = base reg
       simm16[15:0] = immediate

       We choose:
       rD = r3 (dest)
       rA = r0 (base, zero)
       simm16 = 0

       Instruction = (0x20 << 26) | (3 << 21) | (0 << 16) | 0
                   = 0x80 000 00 | (3 << 21)
                   = 0x8000000 + (3 << 21)
                   = 0x8000000 + 0x600000
                   = 0x8600000

       Confirming bits:
       0x20 << 26 = 0x80000000 already too large? Check bits:

       6 bit opcode at bits 31..26 = 0x20 => 100000b
       So left shift opcode by 26 bits.

       0x20 decimal = 32 decimal

       32 << 26 = 32 * 2^26 = 32 * 67108864 = 2147483648 = 0x80000000

       rD=3 << 21 = 3 * 2097152 = 6291456 = 0x600000

       So full instruction = 0x80000000 + 0x600000 + 0 = 0x80600000

       Use this exact instruction:
    */
    asm volatile (
        ".word 0x80600000\n"
        : "=r" (val)
        :
        : "memory"
    );

    return val;
}