#include <stdint.h>

void trigger_carry_flag_clear_signal() {
    __asm__ volatile (
        "l.addi r1, r0, 1\n\t"      // Set r1 to 1
        "l.addi r2, r0, 2\n\t"      // Set r2 to 2
        "l.add r3, r1, r2\n\t"      // Perform addition that could affect the carry flag
        "l.sub r4, r3, r1\n\t"      // Subtract r1 from r3 (no carry expected)
        "l.nop\n\t"
    );
}

int main() {
    trigger_carry_flag_clear_signal();
    return 0;
}