#include <stdint.h>

int main() {
    float a = 5.0f;
    float b = 10.0f;
    float result;

    asm volatile(
        "l.add.s %0, %1, %2\n"       // Floating-point addition
        "l.mfspr r1, r0, 0\n"        // Get the current rounding mode
        : "=f"(result)               // Output result
        : "f"(a), "f"(b)             // Inputs
    );

    return 0;
}