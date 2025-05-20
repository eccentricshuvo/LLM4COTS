#include <stdint.h>

void set_overflow_flag() {
    __asm__ volatile (
        "l.addi r1, r0, 255\n\t"    // Set r1 to a value that will cause overflow
        "l.addi r2, r1, 1\n\t"      // This operation will cause an overflow
        "l.nop\n\t"                 // Simulate the setting of the overflow flag
    );
}

int main() {
    set_overflow_flag();
    return 0;
}