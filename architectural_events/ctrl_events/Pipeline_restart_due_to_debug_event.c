int main(void)
{
    /*
     * Insert the OR1K 'trap' instruction using the proper mnemonic if possible,
     * fallback to encoding with .word 0x00000000 as the trap instruction.
     * This should cause the debug pipeline restart event.
     */
    asm volatile(".word 0x00000000" ::: "memory");

    return 0;
}