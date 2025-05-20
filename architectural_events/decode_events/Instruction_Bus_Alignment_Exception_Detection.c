//int main() {
//    asm volatile (
//        ".word 0x0000000A"  // Instruction that will trigger an instruction bus alignment exception
//    );
//    return 0;
//}

#include <stdint.h>

void misaligned_target(void) {
    asm volatile (".word 0x15000000"); // l.nop for padding
}

// Get address of misaligned_target, add 3 (unaligned)
int main(void) {
    void (*misaligned_fn)(void) = (void *)(((uintptr_t)misaligned_target) + 3);
    misaligned_fn();  // This should trigger decode_except_ibus_align
    return 0;
}

