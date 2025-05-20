int main(void)
{
    /* The original `mtspr` instruction is not recognized by the assembler.
     * We encode the instruction manually and emit it as a .word immediate.
     * 
     * Because 'asm volatile(".word %0" : : "i"(instr));' causes constraint errors
     * due to trying to use a C variable in an immediate constraint,
     * we change to using a constant literal directly in asm.
     *
     * Use the same encoding as before:
     * opcode = 0x1f (6 bits), rD=0, rA=3, spr=1, funct=0x15
     * Instruction encoding: (0x1f << 26) | (0 << 21) | (3 << 16) | (1 << 11) | 0x15
     */

    asm volatile(
        ".word 0x7c220015"
        : /* no outputs */
        : /* no inputs */
    ); /* id=op_mtspr_i */

    return 0;
}