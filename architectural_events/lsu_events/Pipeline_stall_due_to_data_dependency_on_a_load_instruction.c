#include <stdint.h>

volatile uint32_t mem[8] = {
    0xDEADBEEFu, 0xCAFEBABEu, 0x12345678u, 0x0A0B0C0Du,
    0xA5A5A5A5u, 0x5A5A5A5Au, 0xFFFFFFFFu, 0x00000000u
};

int main(void) {
    volatile uint32_t val;
    volatile uint32_t acc = 0;

    /*
     * Strategy: Trigger pipeline stall due to data dependency on load instruction.
     * (1) Load from mem[i & 7]
     * (2) Immediately use loaded value in arithmetic operation
     * This creates a load-use data dependency which should cause pipeline stall.
     */
    for (int i = 0; i < 1024; i++) {
        val = mem[i & 7]; // load instruction
        acc += val;       // dependent arithmetic operation, triggers stall
    }

    return (int)acc; // prevent optimization away
}