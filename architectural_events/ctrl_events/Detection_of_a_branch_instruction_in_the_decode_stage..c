#include <stdint.h>

void detect_branch_instruction() {
    __asm__ volatile (
        "l.addi r1, r0, 1\n\t"      // Set r1 to a value (e.g., 1)
        "l.bne r1, r0, branch_label\n\t" // This is a branch instruction
        "l.nop\n\t"
        "branch_label:\n\t"
        "l.nop\n\t"                 // Simulate the branch target
    );
}

int main() {
    detect_branch_instruction();
    return 0;
}