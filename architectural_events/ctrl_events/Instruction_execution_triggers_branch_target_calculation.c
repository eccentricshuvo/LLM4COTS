#include <stdint.h>

int main(void) {
    // Trigger branch target calculation
    asm volatile(".word 0xc80000b8");  // l.bf 0

    return 0;
}