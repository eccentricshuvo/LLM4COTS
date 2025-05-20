#include <stdint.h>

void trigger_lsu_length_determination() {
    volatile uint8_t byte_var;
    volatile uint16_t halfword_var;
    volatile uint32_t word_var;
    volatile uint64_t dword_var;

    byte_var = 0x12;
    halfword_var = 0x1234;
    word_var = 0x12345678;
    dword_var = 0x1234567890abcdef;

    byte_var = byte_var;
    halfword_var = halfword_var;
    word_var = word_var;
    dword_var = dword_var;
}

int main() {
    trigger_lsu_length_determination();
    return 0;
}