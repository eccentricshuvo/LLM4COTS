#include <stdint.h>

volatile uint32_t memory[4] __attribute__((aligned(4))) = {0};

int main(void) {
    uint32_t value;

    // Trigger alignment exception during a load operation
    asm volatile(
        ".word 0x01018043" : "=r"(value) : "r"(memory)
    );

    return 0;
}