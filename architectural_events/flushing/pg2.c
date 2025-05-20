#include "or1k-sprs.h"
#include "or1k-support.h"

/**
 * @brief Check if the CPU is in Supervisor Mode.
 * @return 1 if in Supervisor Mode, 0 otherwise.
 */
int check_supervisor_mode() {
    uint32_t sr = or1k_mfspr(OR1K_SPR_SYS_SR_ADDR);
    return (sr & OR1K_SPR_SYS_SR_SM_MASK) ? 1 : 0;
}

/**
 * @brief Enable Supervisor Mode if not already enabled.
 */
void enable_supervisor_mode() {
    uint32_t sr = or1k_mfspr(OR1K_SPR_SYS_SR_ADDR);
    sr |= OR1K_SPR_SYS_SR_SM_MASK; // Set Supervisor Mode bit
    or1k_mtspr(OR1K_SPR_SYS_SR_ADDR, sr);
}

/**
 * @brief Initiate a one-time pipeline flush operation.
 */
void trigger_pipeline_flush() {
    // Set flush event using Debug Mode Register 1 (DMR1)
    or1k_mtspr(OR1K_SPR_DEBUG_DMR1_ADDR, OR1K_SPR_DEBUG_DMR1_ST_MASK);

    // Ensure the flush is not persistent by clearing after execution
    or1k_mtspr(OR1K_SPR_DEBUG_DMR1_ADDR, 0x00000000);

    // Explicit pipeline flush via CPU Configuration Register (CPUCFGR)
    or1k_mtspr(OR1K_SPR_SYS_CPUCFGR_ADDR, OR1K_SPR_SYS_CPUCFGR_ND_MASK);

    // Ensure serialization of execution
    __asm__ __volatile__("l.nop 0x0");  // OpenRISC No-Operation

    // Clear CPUCFGR flush bit after setting to avoid continuous triggering
    or1k_mtspr(OR1K_SPR_SYS_CPUCFGR_ADDR, 0x00000000);
}

/**
 * @brief Main function to execute pipeline flush test 10 times.
 */
int main() {
    if (!check_supervisor_mode()) {
        enable_supervisor_mode();
    }

    // Trigger pipeline flush 10 times with clearing after each
    for (int i = 0; i < 10; i++) {
        trigger_pipeline_flush();
        or1k_report(0xF1 + i);  // Debugging report
    }

    // After 10 flushes, execute l.nop indefinitely
    while (1) {
        __asm__ __volatile__("l.nop 0x0");  // Keep CPU executing nops
    }

    return 0;
}

