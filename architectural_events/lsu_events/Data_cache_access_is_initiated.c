#include <stdint.h>

volatile uint32_t mem[8] = {0x12345678u, 0x9ABCDEF0u, 0x0FEDCBA9u, 0x87654321u,
                            0xAAAAAAAAu, 0x55555555u, 0xFFFFFFFFu, 0x00000000u};

int main(void)
{
    for (int i = 0; i < 8; i++) {
        uint32_t val;
        /* Use correct OpenRISC mnemonic casing and registers without symbolic rN */
        register volatile uint32_t *addr_load = &mem[i];
        register volatile uint32_t *addr_store = &mem[(i + 1) & 7];

        __asm__ volatile (
            ".set noreorder\n\t"
            "lw %0, 0(%1)\n\t"       /* Load word from addr_load */
            "sw %0, 0(%2)\n\t"       /* Store word to addr_store */
            ".set reorder\n\t"
            : "=r"(val)
            : "r"(addr_load), "r"(addr_store)
            : "memory"
        );
    }

    return 0;
}