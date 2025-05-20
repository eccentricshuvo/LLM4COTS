int main(void)
{
    volatile int i = 0;
    volatile int limit = 100;
    volatile int target = 50;

    for (i = 0; i < limit; i++) {
        if (i == target) {
            /* Use OpenRISC-1000 exact mnemonics from ISA:
               Branch if equal: beq rA, rB, label (encoding 0x12)
               Add immediate: addi rD, rA, immediate (encoding 0x7)
            */
            __asm__ volatile (
                ".word 0x12000001\n\t" /* beq r17, r19, +1 (1f) */
                ".word 0x70088001\n\t" /* addi r17, r17, 1 */
                "1:\n\t"
                : 
                : "r" (i), "r" (target)
                : "r17", "r19", "memory"
            );
            break;
        }
    }
    return 0;
}