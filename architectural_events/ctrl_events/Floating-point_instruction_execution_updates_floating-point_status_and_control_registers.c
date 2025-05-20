//#include <stdint.h>
//
//int main() {
//    float a = 5.0f;
//    float b = 10.0f;
//    float result;
//
//    asm volatile(
//        "l.adds %0, %1, %2\n"      // Floating-point addition to update FPCSR
//        : "=f"(result)
//        : "f"(a), "f"(b)
//    );
//
//    return 0;
//}
#include <stdint.h>

int main() {
    float a = 1.0f;
    float b = 0.0f;
    float result;

    __asm__ volatile (
        "lf.div.s r1, r2, r3\n"  // Division by zero, triggers Floating Point exception; id: lf.div.s, encoding: opcode 0x32 D A B reserved opcode 0x3
        : 
        : "r" (a), "r" (b)       // Input operands
        : "r1"                   // Output operand
    );

    return 0;
}
