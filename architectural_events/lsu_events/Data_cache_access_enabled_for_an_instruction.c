#include <stdint.h>

/* Volatile variables to force data cache accesses */
volatile uint32_t data_store = 0;
volatile uint32_t data_load = 0;

int main(void)
{
    for (int i = 0; i < 100; i++)
    {
        /* Use OpenRISC-1000 mnemonics from JSON: 
           Store word = sw (syntax: sw rS, imm(rB)) 
           Load word = lw (syntax: lw rD, imm(rB)) 
           Registers are or1k's registers r0-r31, must be used as is in asm 
           We'll name registers via GCC asm constraints accordingly */

        /* Store word via inline asm */
        __asm__ volatile (
            "sw %1, 0(%0)" "\n\t"  /* id: 1000 */
            :
            : "r"(&data_store), "r"(i)
            : "memory"
        );

        uint32_t temp;

        /* Load word via inline asm */
        __asm__ volatile (
            "lw %0, 0(%1)" "\n\t" /* id: 1001 */
            : "=r"(temp)
            : "r"(&data_store)
            : "memory"
        );

        if (temp != (uint32_t)i)
        {
            data_store = 0;
        }
    }

    return (int)data_store;
}