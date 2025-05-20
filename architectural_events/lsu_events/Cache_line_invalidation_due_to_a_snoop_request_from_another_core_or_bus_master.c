#include <stdint.h>
#include <stddef.h>

/* Shared variable aligned to 32 bytes to ensure distinct cache line */
volatile int shared_data __attribute__((aligned(32))) = 0;

/*
 * This program triggers cache line invalidation by performing explicit
 * store word (stw) and load word (ldw) instructions on shared_data using
 * OpenRISC-1000 ISA mnemonics exactly as specified.
 *
 * The 'sync' instruction for memory barrier is not available, so we omit it.
 */

int main(void) {
    int val = 0;

    /* Inline asm for store word: stw value, 0(shared_data) */
    __asm__ volatile (
        "l.movhi r3, %%hi(%0)      \n\t"  /* Load high 16 bits of address */
        "or r3, r3, %%lo(%0)      \n\t"  /* Combine low 16 bits */
        "l.addi r4, r0, 1         \n\t"  /* Load immediate value 1 into r4 */
        "stw r4, 0(r3)            \n\t"  /* Store word r4 to address in r3 */
        :
        : "i" (&shared_data)
        : "r3", "r4", "memory"
    );

    /* Inline asm for load word */
    __asm__ volatile (
        "l.movhi r3, %%hi(%1)     \n\t" /* Load high 16 bits of address */
        "or r3, r3, %%lo(%1)     \n\t" /* Combine low 16 bits */
        "ldw %0, 0(r3)           \n\t" /* Load word from address in r3 into %0 */
        : "=r" (val)
        : "i" (&shared_data)
        : "r3", "memory"
    );

    /* Use val to prevent optimizing out */
    if (val != 1) {
        return 1;
    }

    return 0;
}