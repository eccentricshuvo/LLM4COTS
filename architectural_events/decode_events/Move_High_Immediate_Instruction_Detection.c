#include <stdint.h>

int main(void) {
    uint32_t reg;
    __asm__ volatile (
        "l.movhi %0, 0xABCD\n\t"
        : "=r" (reg)
    );
    return 0;
}