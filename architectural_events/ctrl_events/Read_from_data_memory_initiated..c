#include <stdint.h>

int main() {
    volatile uint32_t* addr = (uint32_t*) 0x10000000;
    uint32_t data = *addr;
    return 0;
}