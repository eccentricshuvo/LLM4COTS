#include <stdio.h>

int main() {
    int rA = 10;
    int rB = 5;
    int rD;

    // Perform bitwise XOR operation and set conditional flags
    asm volatile (
        "l.xor %0, %1, %2"   // rD ← rA XOR rB
        : "=r" (rD) : "r" (rA), "r" (rB)
    );

    return 0;
}