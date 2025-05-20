#include <stdint.h>

void indicate_decode_bubble() {
    __asm__ volatile (
        "l.nop\n\t"                // Create a bubble in the pipeline
        "l.nop\n\t"                // Additional nops to simulate the stall condition
        "l.nop\n\t"
    );
}

int main() {
    indicate_decode_bubble();
    return 0;
}