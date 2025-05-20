#include <stdint.h>

volatile uint32_t mem[4] = {0x12345678u, 0x9ABCDEF0u, 0x0u, 0x0u};

int main(void)
{
    uint32_t val;

    // Load value from memory (trigger load access_done)
    val = mem[0];

    // Store value to memory (trigger store access_done)
    mem[2] = val;

    // Load again to exercise load access_done
    val = mem[2];

    // Store again (trigger store access_done)
    mem[3] = val;

    // Prevent optimizer from removing memory operations
    return (int)val;
}