#include <stdio.h>

int main() {
    // Emit an instruction with undefined opcode (e.g., 0xFFFFFFFF)
    asm volatile (".word 0xFFFFFFFF");  // Illegal opcode for OR1K
    return 0;
}
