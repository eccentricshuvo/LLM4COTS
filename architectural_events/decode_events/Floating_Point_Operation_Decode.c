#include <stdint.h>

void trigger_floating_point_operation_decode() {
    __asm__ volatile (
        "l.nop\n\t"               // No operation to set context 
        "l.nop\n\t"               // No operation as placeholder
        ".word 0xE2000000\n\t"    // Simulated floating-point operation instruction
        "l.nop\n\t"
    );
}

int main() {
    trigger_floating_point_operation_decode();
    return 0;
}