#include <stdint.h>

void trigger_zero_extended_immediate_selection() {
    __asm__ volatile (
        "l.addi r1, r0, 1\n\t"      // Add immediate (1) to r0 to set r1 to 1
        "l.addi r2, r0, 0xFF\n\t"   // Add immediate (255), should trigger zero extension for any further use
        "l.nop\n\t"
    );
}

int main() {
    trigger_zero_extended_immediate_selection();
    return 0;
}