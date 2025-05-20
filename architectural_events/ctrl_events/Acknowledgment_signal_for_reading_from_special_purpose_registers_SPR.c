#include <stdint.h>

int main() {
    asm volatile(
        "l.mfspr r5, r0, 0"
    );

    return 0;
}