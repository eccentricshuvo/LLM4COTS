#include <stdint.h>

volatile uint32_t mem[2] = {1, 2};

int main(void) {
    volatile uint32_t *ptr = mem;
    uint32_t a, b;

    a = ptr[0];    // load from memory (volatile to force actual load)
    b = a + 1;     // dependent operation to create hazard

    volatile uint32_t sink = b;
    return (int)sink;
}