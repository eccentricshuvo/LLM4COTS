#include <stdint.h>

int main(void) {
    uint32_t a = 0xFFFFFFFF;
    uint32_t b = 0x1;
    uint32_t result;
    
    __asm__ volatile (
        "l.addc %0, %1, %2\n\t"
        : "=r" (result)
        : "r" (a), "r" (b)
    );
    
    return 0;
}