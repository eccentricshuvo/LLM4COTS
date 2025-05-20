int main(void)
{
    /* Trigger a memory synchronization operation using inline assembly.
     * The OpenRISC-1000 "msync" instruction encoding is 0x45000000.
     * Using .word to encode this instruction as assembler doesn't recognize mnemonic.
     */

    __asm__ volatile(
        ".word 0x45000000" /* msync instruction */
    );

    return 0;
}