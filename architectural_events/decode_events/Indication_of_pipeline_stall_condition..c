#include <stdint.h>

void trigger_pipeline_stall() {
    __asm__ volatile (
        "l.nop\n\t"                // No operation to create a bubble in the pipeline
        "l.nop\n\t"                // Additional nop to induce stall condition
        "l.nop\n\t"                // Further nop to illustrate the stall
    );
}

int main() {
    trigger_pipeline_stall();
    return 0;
}