#include <stdint.h>

void trigger_pipeline_stall() {
    __asm__ volatile (
        "l.nop\n\t"                // Create a bubble in the pipeline
        "l.nop\n\t"                // Additional nop to ensure the stall condition is met
        "l.nop\n\t"                // Further nop to prolong the stall
    );
}

int main() {
    trigger_pipeline_stall();
    return 0;
}