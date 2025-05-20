#include <stdint.h>

volatile uint32_t buffer[128];

int main(void)
{
    // Initialize buffer with some values to ensure valid memory access
    for (int i = 0; i < 128; i++)
        buffer[i] = i * 7;

    // Access buffer with stride 1 to repeatedly hit cache lines and force way match
    for (int i = 0; i < 128; i++)
    {
        volatile uint32_t val;

        // Use pure C loads/stores (lw/sw are implicit in C, no inline asm)
        val = buffer[i];

        // Modify val and store back
        buffer[i] = val + 1;

        // Insert 'l.msync' instruction inline to enforce memory operation order
        // 'l.msync' is the OpenRISC mnemonic for memory barrier
        __asm__ volatile ("l.msync" ::: "memory");
    }

    return 0;
}