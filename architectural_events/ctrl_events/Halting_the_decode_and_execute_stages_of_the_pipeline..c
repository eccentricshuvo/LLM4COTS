#include <stdint.h>

void halt_decode_execute_stages() {
    __asm__ volatile (
        "l.nop\n\t"                // Create a stall in the pipeline
        "l.nop\n\t"                // Additional nops to simulate halting conditions
        "l.nop\n\t"
        "l.addi r1, r0, 0\n\t"     // Set r1 to 0 (no operation to simulate halt condition)
    );
}

int main() {
    halt_decode_execute_stages();
    return 0;
}