#include <stdint.h>

int main(void) {
    __asm__ volatile (
        ".word 0xFC000000\n\t"
    );
    return 0;
}