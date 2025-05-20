#include <stdint.h>

void trigger_carry_set_signal() {
    __asm__ volatile (
        "l.addi r1, r0, 255\n\t"    // Set r1 to a value that will generate a carry
        "l.addi r2, r1, 1\n\t"      // Add 1 to r1, setting the carry condition
        "l.nop\n\t"
    );
}

int main() {
    trigger_carry_set_signal();
    return 0;
}