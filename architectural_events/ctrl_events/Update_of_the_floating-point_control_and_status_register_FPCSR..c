#include <stdint.h>

int main() {
    uint32_t ctrl_fpcsr_i = 0x123;  // Example value for ctrl_fpcsr_i
    uint32_t fpcsr_register;

    asm("l.mtspr %0, %1, 5" : "=r"(fpcsr_register) : "r"(ctrl_fpcsr_i));

    return 0;
}