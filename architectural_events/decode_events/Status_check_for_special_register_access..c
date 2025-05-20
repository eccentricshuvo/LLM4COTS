#include <stdint.h>

void trigger_special_register_access_check() {
    __asm__ volatile (
        "l.mfspr r1, 0\n\t"        // Move from special purpose register (SPR) to r1
        "l.mtspr 0, r1\n\t"       // Move to special purpose register, triggering access check
        "l.nop\n\t"
    );
}

int main() {
    trigger_special_register_access_check();
    return 0;
}