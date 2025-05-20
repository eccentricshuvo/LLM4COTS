#include <stdint.h>

int main(void) {
    volatile uint32_t array[2] = {0}; // Array to trigger memory access
    uint32_t dummy;

    // Inline assembly with load and store instructions to trigger control flow validation
    asm volatile (
        "lwa %[dummy], 0(%[array])\n"   // Load word instruction
        "swa %[dummy], 4(%[array])\n"    // Store word instruction
        : [dummy] "+r" (dummy)           // Output and input operand
        : [array] "r" (array)            // Array operand
    );

    return 0;
}