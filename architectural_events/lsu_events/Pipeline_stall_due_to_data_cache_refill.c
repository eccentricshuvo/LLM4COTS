#include <stdint.h>

#define ARRAY_SIZE 4096

volatile uint32_t large_array[ARRAY_SIZE];

int main(void)
{
    // Initialize large_array with some values to avoid optimisation removing loads
    for (int i = 0; i < ARRAY_SIZE; ++i)
        large_array[i] = i * 3 + 7;

    volatile uint32_t acc = 0;

    for (int i = 0; i < ARRAY_SIZE; i += 64)
    {
        uint32_t val;
        // Load the address into a temporary register and then load word from it
        // Use OpenRISC mnemonics exactly as per ISA: lw rD, imm(rA)
        // Use inline asm with two instructions:
        // 1) "or r8, r0, %1" to load address into r8 (r8 is caller-saved)
        // 2) "lw %0, 0(r8)" to load value into val

        __asm__ volatile (
            "or %%r8, r0, %1\n"   // r0 is zero register, %1 is input pointer
            "lw %0, 0(r8)\n"
            : "=r"(val)
            : "r"(&large_array[i])
            : "r8"
        );
        acc += val;
    }

    return (int)acc;
}