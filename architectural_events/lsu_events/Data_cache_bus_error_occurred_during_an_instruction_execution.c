int main(void)
{
    /* Trigger data cache bus error by accessing invalid memory address 0.
     * According to the OpenRISC ISA JSON, mnemonics for load word and store word are:
     *  - lwz (Load Word Zero)
     *  - sw  (Store Word)
     * 
     * The assembler errors indicate the previous usage was incorrect.
     * The correct syntax is: "lwz rD, offset(rA)" and "sw rS, offset(rA)".
     * Registers are r3 and r0.
     * Access memory at address 0 (offset 0 + r0).
     */
    asm volatile(
        "lwz r3,0(r0)\n"  /* load word zero: lwz r3,0(r0) */
        "sw r3,0(r0)\n"   /* store word: sw r3,0(r0) */
        :
        :
        : "r3"
    );

    return 0;
}