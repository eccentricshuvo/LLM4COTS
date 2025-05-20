#include <stdint.h>

volatile int32_t mem[8] = { 10, 20, 30, 40, 50, 60, 70, 80 };

int main(void)
{
    /* Perform a sequence of loads and stores to provoke dbus_access signal */
    for (int i = 0; i < 8; ++i)
    {
        int32_t val;
        /* Load operation with inline asm using the OpenRISC lw syntax: lw rD, imm(rA)
           We use %0 for the destination register and %1 for the memory address label */
        __asm__ volatile (
            "lw %0, 0(%1)\n"            /* lw rD, 0(rA) */
            : "=r" (val)
            : "r" (&mem[i])
            : );

        /* Store operation with inline asm using the OpenRISC sw syntax: sw rS, imm(rA) */
        __asm__ volatile (
            "sw %0, 0(%1)\n"            /* sw rS, 0(rA) */
            :
            : "r" (val + i), "r" (&mem[i ^ 1])
            : );
    }

    return 0;
}