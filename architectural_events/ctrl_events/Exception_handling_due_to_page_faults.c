#include <stdint.h>

volatile uint32_t data[8] = {0};

int main(void) {
    uint32_t result = 0;

    for (int i = 0; i < 8; ++i) {
        data[i] = i;  // Memory access to provoke a page fault
    }

    // The load instruction will trigger the data page fault exception
    // which corresponds to the except_dpagefault_i signal
    asm volatile(".word 0x47030000" :: "r"(&data[9]) : "r3");

    return 0;
}