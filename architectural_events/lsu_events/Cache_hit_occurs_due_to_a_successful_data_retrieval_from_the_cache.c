#include <stdint.h>

volatile uint32_t data = 0x12345678u;

int main(void)
{
    /* Access the same memory location repeatedly to cause a cache hit.
       Use OpenRISC explicit load (l.lwz) and store (l.sw) instructions via inline assembly
       with correct mnemonics, and use addi as addi_0 (addi with zero register) substitution with proper syntax.
       Since addi is not recognized, replace increment with addi_0 format or use explicit arithmetic.
    */
    for (int i = 0; i < 100000; ++i)
    {
        uint32_t val;
        __asm__ volatile (
            "l.lwz %0, 0(%1) \n\t"           /* Load word from memory */
            /* Increment val by 1 using addi_0: addi rX, rX, 1 is addi with dst=src+imm */
            /* If addi is unrecognized, use OR instruction to add immediate 1 */
            /* OR immediate not in listed mnemonics, so use addi_0 mnemonic */
            "addi_0 %0, %0, 1 \n\t"          /* Increment val by 1 */
            "l.sw  %0, 0(%1) \n\t"           /* Store word back */
            : "=&r"(val)
            : "r"(&data)
            : "memory"
        );
    }

    /* Return the final value to prevent optimization */
    return (int)data;
}