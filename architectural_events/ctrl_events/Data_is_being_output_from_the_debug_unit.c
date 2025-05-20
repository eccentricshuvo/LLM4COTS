int main(void)
{
    /* The assembler does not recognize 'break' as an instruction mnemonic,
       so we use the .word directive with the encoding of the 'break' instruction:
       0100 0000 0000 0000 0000 0000 0000 0000 binary = 0x40000000 hex */

    asm volatile(".word 0x40000000");  /* break instruction encoding */

    return 0;
}