/* OpenRISC‑1000 test: provoke decode_bubble_o via a classic load‑use RAW hazard.
 *
 * How it works
 * ------------
 *  1.  Each loop iteration performs:
 *        lwz   r3,0(r4)     ; load word from memory
 *        add   r5,r3,r5     ; immediately uses r3 → load‑use dependence
 *      When the load is in the execute stage and the ADD is in decode,
 *      the register file has not yet been written, so decode_bubble_o
 *      is asserted for one cycle to stall the pipeline.
 *
 *  2.  The loop repeats 50 000 times, giving thousands of bubbles
 *      while staying well under the 100 k‑instruction budget.
 *
 *  3.  No host I/O, no branch-to-register, no atomics—pure load‑use.
 */

#include <stdint.h>

/* Put the word in uncached memory (volatile) so the compiler cannot optimise
   the load away or combine iterations. */
volatile uint32_t mem_word = 0xDEADBEEF;

int main(void)
{
    /* r_acc will accumulate the loaded value so the compiler
       keeps both the load and the subsequent dependent ADD.   */
    uint32_t r_acc = 0;

    /* 2 instructions per iteration → 100 000 / 2 = 50 000 iterations max. */
    for (int i = 0; i < 5000; ++i)
    {
        /* ---- RAW hazard pair ---- */
        uint32_t t = mem_word; /* load (goes to execute) */
        r_acc += t;            /* uses same reg next cycle → decode needs it */
    }

    /* Prevent entire loop from being optimised away */
    return (int)r_acc;
}

