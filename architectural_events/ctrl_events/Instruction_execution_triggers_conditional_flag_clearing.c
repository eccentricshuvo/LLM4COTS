#include <stdint.h>

// Assume base address of OMPIC control registers
#define OMPIC_BASE 0xF0000000

// Control register offsets
#define CONTROL_REG(offset) (*(volatile uint32_t *)(OMPIC_BASE + (offset)))
#define IRQ_CONTROL 0x0 // Offset for control register

void trigger_irq(uint8_t dst_core, uint32_t data) {
    // Writing to control register to generate IRQ
    CONTROL_REG(IRQ_CONTROL) = (1 << 31) | (1 << 29) | (dst_core & 0x1FFF) | (data & 0xFFFFFFFF);
}

void acknowledge_irq() {
    // Acknowledge the interrupt
    CONTROL_REG(IRQ_CONTROL) = (1 << 31);
}

int main() {
    trigger_irq(0x1, 0xDEADBEEF); // Custom IRQ to core 1 with data
    acknowledge_irq(); // Acknowledge the IRQ
    return 0;
}