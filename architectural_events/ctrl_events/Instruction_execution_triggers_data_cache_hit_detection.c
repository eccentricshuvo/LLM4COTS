#include <stdint.h>

volatile uint32_t data[8] __attribute__((section(".data"))) = {0x12345678, 0xAABBCCDD, 0x55667788, 0x11223344, 0xDEADBEEF, 0xCAFEBABE, 0xABCDEF01, 0x98765432};

int main(void) {
    uint32_t sum = 0;

    for (int i = 0; i < 8; i++) {
        sum += data[i];
    }

    __asm__ __volatile__ (
        "l.nop 0\n\t"   // Perform a pipeline stall for cache coherence
        ::: "memory"
    );

    return sum;
}