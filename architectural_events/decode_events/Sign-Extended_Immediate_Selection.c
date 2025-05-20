#include <stdint.h>

void trigger_sign_extended_immediate_selection() {
    __asm__ volatile (
        "l.addi r1, r0, -1\n\t"   // Add immediate to r0 (which is 0), triggering sign extension
        "l.nop\n\t"
    );
}

int main() {
    trigger_sign_extended_immediate_selection();
    return 0;
}