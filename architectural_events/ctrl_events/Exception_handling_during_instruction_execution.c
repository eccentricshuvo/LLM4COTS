#include <stdint.h>

volatile uint32_t data_array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

int main(void) {
    // Access memory and trigger exception handling
    uint32_t result = data_array[0] + data_array[1];

    // Inline assembly to ensure memory access triggers exception handling
    asm volatile (
        "l.ori r3, r0, %0\n"    // Load immediate value into r3
        "l.lwz r4, 0(r3)\n"     // Load word from memory into r4
        "l.add r5, r4, r0\n"     // Add memory value to r5
        "l.sw 0(r3), r5\n"      // Store back the result to memory
        :
        : "r"(&data_array)
        : "r3", "r4", "r5"
    );

    return 0;
}