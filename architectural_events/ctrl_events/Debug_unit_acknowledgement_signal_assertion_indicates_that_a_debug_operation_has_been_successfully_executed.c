int main(void)
{
    /* The OpenRISC 'break' instruction mnemonic may be different or may require encoding.
       Use machine code encoding for the 'break' instruction: 0x00000200 (break immediate 0).
       Using .word directive to emit break instruction directly.
    */

    __asm__ volatile (".word 0x00000200"); // 'break 0' instruction encoding

    return 0;
}