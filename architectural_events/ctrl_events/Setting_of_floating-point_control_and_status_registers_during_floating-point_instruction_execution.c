#include <stdint.h>

int main() {
    float a = 5.0f;
    float b = 10.0f;
    float result;

    asm volatile(
        "l.add.s r1, %0, %1\n"      // Floating-point addition
        "l.mtspr r0, r1, 0\n"       // Explicitly update FPCSR
        : 
        : "r"(a), "r"(b)             // Use "r" constraint for register
    );

    return 0;
}