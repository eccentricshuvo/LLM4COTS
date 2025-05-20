#include <stdint.h>

void initiate_move_from_spr() {
    __asm__ volatile (
        "l.mfspr r1, 0\n\t"        // Move from special purpose register (assumed SPR 0) to r1
        "l.nop\n\t"                // Simulate acknowledgment of the operation
    );
}

int main() {
    initiate_move_from_spr();
    return 0;
}