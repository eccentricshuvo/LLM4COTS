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
 * @brief Initiate a pipeline flush operation.
 */
void trigger_pipeline_flush() {
    // Use correct macro for Debug Mode Register 1 (DMR1)
    or1k_mtspr(OR1K_SPR_DEBUG_DMR1_ADDR, OR1K_SPR_DEBUG_DMR1_ST_MASK);

    // Explicit pipeline flush via CPU configuration register
    or1k_mtspr(OR1K_SPR_SYS_CPUCFGR_ADDR, OR1K_SPR_SYS_CPUCFGR_ND_MASK);

    // Ensure instruction serialization
    __asm__ __volatile__("l.nop 0x0");  // OpenRISC No-Operation
}

/**
 * @brief Main function to execute pipeline flush test.
 */
int main() {
    if (!check_supervisor_mode()) {
        enable_supervisor_mode();
    }

    // Trigger pipeline flush
    trigger_pipeline_flush();

    // Verify if the pipeline flush operation was successful
    uint32_t status = or1k_mfspr(OR1K_SPR_SYS_CPUCFGR_ADDR);
    if (status & OR1K_SPR_SYS_CPUCFGR_ND_MASK) {
        or1k_report(0xF1);  // Custom report indicating pipeline flush success
    }

    return 0;
}

