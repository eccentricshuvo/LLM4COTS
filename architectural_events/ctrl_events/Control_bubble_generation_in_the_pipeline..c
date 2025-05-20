#include <stdint.h>

void trigger_control_bubble_generation() {
    __asm__ volatile (
        "l.nop\n\t"                // Create a control bubble in the pipeline
        "l.nop\n\t"                // Additional nops to propagate the bubble
        "l.nop\n\t"
    );
}

int main() {
    trigger_control_bubble_generation();
    return 0;
}