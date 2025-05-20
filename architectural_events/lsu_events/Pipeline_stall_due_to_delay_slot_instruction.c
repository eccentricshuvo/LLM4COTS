int main(void)
{
    volatile int i = 0;
    volatile int x = 0;

    /*
     * OpenRISC-1000 ISA mnemonics must be exact:
     * - Use "l.addi" for add immediate
     * - Use "l.bf" or "l.bnf" for branch
     * - Use "l.movhi" with immediate or "l.mov rD, rA" only with registers, but here best to use registers correctly.
     * - No pseudo-mnemonics like l.mov for reg->reg moves, use l.add rD, rA, r0 instead (add zero to move).
     *
     * To trigger pipeline stall due to delay slot instruction,
     * we build a loop where branch with delay slot is followed by increment.
     */

    asm volatile (
        /* Move i to r3 by adding zero */
        "l.add r3, %0, r0      \n\t"    // r3 = i

        "1:                    \n\t"
        "l.addi r4, r0, 10     \n\t"    // r4 = 10
        "l.sub r5, r4, r3      \n\t"    // r5 = 10 - i
	"l.sfeq r5, r0	       \n\t"	// set flag if r5 == 0
        "l.bf 2f               \n\t"    // Branch if false (r5 == 0) to label 2
        "l.addi r3, r3, 1      \n\t"    // delay slot: increment i

        "l.j 1b                \n\t"    // jump back to label 1

        "2:                    \n\t"
        "l.add %0, r3, r0      \n\t"    // Move r3 back to i by adding zero

        : "+r" (i)
        :
        : "r3", "r4", "r5"
    );

    x = i;

    return x;
}
