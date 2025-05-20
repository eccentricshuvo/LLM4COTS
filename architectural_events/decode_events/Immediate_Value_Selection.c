#include <stdint.h>

void trigger_immediate_value_selection() {
    __asm__ volatile (
        "l.movhi r1, 0xABCD\n\t" // Move high immediate value into r1
        "l.nop\n\t"
    );
}

int main() {
    trigger_immediate_value_selection();
    return 0;
}