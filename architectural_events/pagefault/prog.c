#include <stdio.h>
#include <stdlib.h>

// Define Special Purpose Registers (SPR)
#define SPR_SR      0x17   // Supervision Register
#define SPR_EVBAR   0x11   // Exception Vector Base Address Register

// Function to write to a SPR
static inline void mtspr(uint32_t spr, uint32_t value) {
    __asm__ volatile ("l.mtspr r0, %0, %1" :: "r"(value), "I"(spr));
}

// Function to read from a SPR
static inline uint32_t mfspr(uint32_t spr) {
    uint32_t value;
    __asm__ volatile ("l.mfspr %0, r0, %1" : "=r"(value) : "I"(spr));
    return value;
}

int main() {
    printf("Enabling MMU and triggering instruction page fault...\n");

    // 🔹 Enable the MMU (Set IME bit in SR)
    uint32_t sr = mfspr(SPR_SR);
    sr |= (1 << 6);  // IME bit (Instruction MMU Enable)
    mtspr(SPR_SR, sr);

    // 🔹 Attempt to execute an invalid instruction address
    void (*invalid_func)() = (void(*)()) 0xDEADBEEF;
    invalid_func();  // Should trigger instruction page fault

    // If execution continues past this point, the fault did not trigger
    printf("ERROR: Instruction page fault did not trigger!\n");
    exit(1);

    return 0;
}
