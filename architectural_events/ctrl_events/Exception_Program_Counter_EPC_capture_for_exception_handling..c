#include <stdint.h>

int main() {
    uint32_t exception_address;
    asm("l.mfspr %0, r0, %1" : "=r"(exception_address) : "i"(1));
    
    return 0;
}