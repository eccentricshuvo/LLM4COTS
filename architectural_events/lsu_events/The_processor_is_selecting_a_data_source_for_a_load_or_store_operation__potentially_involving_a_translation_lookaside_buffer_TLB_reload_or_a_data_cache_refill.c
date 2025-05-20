#include <stdint.h>

#define ARRAY_SIZE 256

volatile uint32_t array[ARRAY_SIZE];
volatile uint32_t sink;

int main(void) {
    // Initialize array with some pattern
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = (uint32_t)(i * 3 + 7);
    }

    // Access memory with loads and stores to provoke TLB and dcache activity:
    // Use varying addresses and data sizes with volatile accesses to ensure actual load/store instructions

    for (int i = 0; i < ARRAY_SIZE; i++) {
        // Load word (32-bit)
        uint32_t val = array[i];

        // Store word (32-bit)
        array[(i + 13) % ARRAY_SIZE] = val ^ 0xA5A5A5A5u;

        // Load byte: force a different load size, potentially crossing cache lines
        volatile uint8_t *byte_ptr = (volatile uint8_t *)&array[(i + 7) % ARRAY_SIZE];
        uint8_t byte_val = *byte_ptr;

        // Store byte back
        *byte_ptr = byte_val ^ 0xFFu;

        // Combine values to avoid compiler optimizing away
        sink ^= val + byte_val;
    }

    // Additional non-aligned access pattern for variation and provoke TLB/cache line behavior
    for (int i = 1; i < ARRAY_SIZE - 1; i++) {
        volatile uint16_t *half_ptr = (volatile uint16_t *)((uintptr_t)&array[i] + 1);
        uint16_t half_val = *half_ptr;
        *half_ptr = half_val ^ 0x55AAu;

        sink ^= half_val;
    }

    // Avoid optimizing away sink variable
    return (int)sink;
}