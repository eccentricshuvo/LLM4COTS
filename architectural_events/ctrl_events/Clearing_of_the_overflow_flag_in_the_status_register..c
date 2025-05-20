#include <stdint.h>

void clear_overflow_flag() {
    __asm__ volatile (
        "l.addi r1, r0, 0\n\t"      // Set r1 to 0 to simulate the condition for clearing overflow
        "l.mfspr r2, 0\n\t"         // Move from special purpose register to r2 (assumed status register)
        "l.and r2, r2, 0xFFFFFFFE\n\t" // Clear the overflow flag (assuming it's bit 0)
        "l.mtspr 0, r2\n\t"         // Move updated value back to the status register (assumed SPR 0)
        "l.nop\n\t"
    );
}

int main() {
    clear_overflow_flag();
    return 0;
}