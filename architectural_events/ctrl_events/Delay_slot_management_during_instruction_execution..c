#include <stdint.h>

void manage_delay_slot() {
    __asm__ volatile (
        "l.addi r1, r0, 1\n\t"      // Set r1 to 1 to simulate a condition for the delay slot operation
        "l.j 1f\n\t"                // Jump to label 1 (creating a delay slot)
        "l.nop\n\t"                 // This nop acts as the instruction in the delay slot
        "1:\n\t"
        "l.addi r2, r0, 2\n\t"      // Set r2 to 2, simulating normal execution after the delay
        "l.nop\n\t"
    );
}

int main() {
    manage_delay_slot();
    return 0;
}