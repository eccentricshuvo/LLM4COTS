#include <stdint.h>

int main() {
    uint32_t a = 5;
    uint32_t b = 10;

    asm volatile(
        "l.sub r1, %0, %1\n"       // Subtract to set conditional flags
        "l.mfspr r0, r0, 0\n"      // Get current flags state
        "l.add r2, r2, r2\n"       // Dummy operation to keep pipeline flow
        : 
        : "r"(a), "r"(b)
    );

    return 0;
}