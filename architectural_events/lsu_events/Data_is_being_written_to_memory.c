#include <stdint.h>

volatile uint32_t mem[4] = {0, 0, 0, 0};

int main(void) {
    // Store values to memory to trigger data write (cpu_dat_o) signal
    for (uint32_t i = 0; i < 4; ++i) {
        mem[i] = i * 0x11111111u;
    }

    // Read back to prevent compiler optimization removing stores
    volatile uint32_t tmp = 0;
    for (uint32_t i = 0; i < 4; ++i) {
        tmp += mem[i];
    }
    return (int)tmp;
}