#include <stdint.h>

void trigger_load_store_unit_zero_extension() {
    uint8_t byte_value = 0xAB; // Arbitrary byte value for testing

    __asm__ volatile (
        "l.sw %0, 0(r0)\n\t"       // Store the byte value in memory
        "l.nop\n\t"
        "l.lbu r1, 0(r0)\n\t"      // Load byte unsigned from address 0 (zero extension)
        "l.nop\n\t"
        : 
        : "r" (byte_value)
    );
}

int main() {
    trigger_load_store_unit_zero_extension();
    return 0;
}