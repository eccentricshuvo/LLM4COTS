#include <stdint.h>

volatile uint32_t data[4] = {0x12345678, 0x9ABCDEF0, 0x0, 0xFFFFFFFF};

int main(void) {
    volatile uint32_t x;

    // Access the same memory address twice consecutively to provoke cpu_adr_match_i
    x = data[1];  // First load from data[1]
    x = data[1];  // Second load from same address, triggers address match

    // Modify address slightly and access again
    x = data[2];  // Different address
    x = data[2];  // Same address again

    // Store followed by store to same address to create matching access
    data[3] = 0xA5A5A5A5;
    data[3] = 0x5A5A5A5A;

    return 0;  // Prevent optimizer removal
}