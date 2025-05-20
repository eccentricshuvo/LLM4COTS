#include <stdint.h>

int main(void) {
    // Trigger branch decoding
    asm volatile("l.bf 0");  // Branch forward if flag set

    return 0;
}