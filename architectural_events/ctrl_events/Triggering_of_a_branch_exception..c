#include <stdint.h>

void trigger_branch_exception() {
    __asm__ volatile (
        "l.addi r1, r0, 0\n\t"       // Set r1 to 0
        "l.bnez r1, branch_target\n\t" // Branch if r1 is not zero (will not take the branch)
        "l.nop\n\t"
        "branch_target:\n\t"
        "l.addi r2, r0, 1\n\t"       // Change the flow for the exception scenario
        "l.bnez r2, exception_label\n\t" // Branch to exception label
        "l.nop\n\t"
        "exception_label:\n\t"
        "l.nop\n\t"
    );
}

int main() {
    trigger_branch_exception();
    return 0;
}