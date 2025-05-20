int main(void)
{
    unsigned int value;
    /* Use inline assembly with .word encoding for mfspr r3,1 to bypass assembler error.
       Instead of 'mov' (unrecognized), use 'or %0, r3, r0' to copy r3 to output register (per OpenRISC ISA).
       
       Encoding of mfspr r3,1:
       opcode = 0x76
       rD = 3 (bits 21-25 = 00011)
       SPR = 1 (bits 16-20 = 00001)
       Final word = (3 << 21) | (1 << 16) | 0x76 = 0x06100076
       
       Then use 'or' instruction:
       or %0, r3, r0  (r0 is zero register)
    */

    asm volatile(
        ".word 0x06100076 /* mfspr r3,1 */ \n"
        "or %0, r3, r0   /* copy r3 to output */"
        : "=r"(value)
        :
        : "r3");
    return (int)value;
}