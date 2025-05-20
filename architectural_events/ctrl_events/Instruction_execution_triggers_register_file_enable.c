#include <stdint.h>

int main(void) {
    uint32_t dummy;

    // Minimal inline assembly to trigger register file enable
    asm volatile (
        "l.add %0, r0, r0"  // Use a simple arithmetic operation
        : "=r" (dummy)  // Output operand
    );

    return 0;
}