#include <stdint.h>

void trigger_branch_misprediction() {
    __asm__ volatile (
        "l.jal branch_target\n\t"
        "l.nop\n\t"
        "branch_target:\n\t"
        "l.jr r9\n\t"
        "l.nop\n\t"
    );
}

int main() {
    trigger_branch_misprediction();
    return 0;
}