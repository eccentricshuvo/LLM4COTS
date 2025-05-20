#include <stdint.h>

volatile uint32_t mem[4] = {0x12345678u, 0x9ABCDEF0u, 0x0u, 0xFFFFFFFFu};

int main(void) {
    volatile uint32_t val;

    // Perform multiple loads and stores to provoke cpu_adr_i signal:
    // Load from mem[0]
    val = mem[0];
    // Store to mem[1]
    mem[1] = val;
    // Load from mem[2]
    val = mem[2];
    // Store to mem[3]
    mem[3] = val;

    // Return value to avoid optimization away
    return (int)val;
}