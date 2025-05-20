/* Test program to deliberately cause a CPU error signal (cpu_err_o) on OpenRISC 1000
 * by attempting to access an invalid/unmapped memory address and also triggering
 * a load/store exception via misaligned address access.
 *
 * Strategy:
 *   - Access an invalid/unmapped memory address to cause cache protection exception
 *   - Perform a deliberately misaligned store using the correct OpenRISC instruction mnemonic from ISA doc
 *   - Use volatile pointers and inline asm for specific instructions to trigger event
 */

int main(void)
{
    /* Out-of-range/invalid memory address likely to cause cache error */
    volatile unsigned int *ptr_invalid = (unsigned int *)0xF0000000u;

    /* Read from invalid address */
    volatile unsigned int val = *ptr_invalid;

    /* Write to invalid address */
    *ptr_invalid = val;

    /* Additionally, try misaligned store using inline asm to trigger cache/exception fault more reliably */
    unsigned int misaligned_val = 0xDEADBEEF;
    unsigned int *misaligned_ptr = (unsigned int *)((char *)&misaligned_val + 1); // deliberately misaligned

    /* OpenRISC-1000 mnemonic for store word is "stw" */
    __asm__ volatile (
        "stw %0, 0(%1)\n" /* Store Word at misaligned address to provoke fault */
        : /* no outputs */
        : "r"(misaligned_val), "r"(misaligned_ptr)
        : "memory"
    );

    return (int)val;
}