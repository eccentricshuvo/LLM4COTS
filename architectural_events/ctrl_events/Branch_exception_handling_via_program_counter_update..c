#include <stdint.h>

void trigger_branch_exception_handling() {
    __asm__ volatile (
        "l.addi r1, r0, 1\n\t"      // Set r1 to 1
        "l.bnez r1, branch_target\n\t" // Branch to target if r1 is not zero
        "l.nop\n\t"
        "branch_target:\n\t"
        "l.addi r2, r0, 2\n\t"      // Set r2 to 2, simulating an exception scenario
        "l.nop\n\t"
    );
}

int main() {
    trigger_branch_exception_handling();
    return 0;
}