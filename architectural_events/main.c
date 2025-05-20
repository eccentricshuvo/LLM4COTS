#include "or1k-sprs.h"
#include "or1k-support.h"
#define SPR_SR 17  // Supervision Register (SR) index from OpenRISC manual
#define TEST_VIRT_ADDR  0x40000000  // Example virtual address
#define TEST_PHYS_ADDR  0x20000000  // Example physical address
#define TEST_TLB_INDEX  0  // Using index 0 for TLB entry

void enable_immu() {
    uint32_t sr;
    // Read the current value of the Supervision Register (SR)
    sr = or1k_mfspr(SPR_SR);
    // Use the provided macro to enable IMMU
    sr = OR1K_SPR_SYS_SR_IME_SET(sr, 1);
    // Write the modified value back to the SR
    or1k_mtspr(SPR_SR, sr);
}


//void clear_itlb() {
//    uint32_t immucr = or1k_mfspr(OR1K_SPR_IMMU_IMMUCR_ADDR);
//    immucr = OR1K_SPR_IMMU_IMMUCR_ITF_SET(immucr, 0);  // Clear ITLB flush bit
//    immucr |= (1 << 1);  // Set IME (Instruction MMU Enable) bit
//    or1k_mtspr(OR1K_SPR_IMMU_IMMUCR_ADDR, immucr);
//}

void clear_itlb() {
    uint32_t immucr = or1k_mfspr(OR1K_SPR_IMMU_IMMUCR_ADDR);
    immucr = OR1K_SPR_IMMU_IMMUCR_ITF_SET(immucr, 0);  // Clear ITLB flush bit
    or1k_mtspr(OR1K_SPR_IMMU_IMMUCR_ADDR, immucr);

    // Enable IMMU in the correct SR register
    uint32_t sr = or1k_mfspr(OR1K_SPR_SYS_SR_ADDR);
    sr |= (1 << 1);  // Set IME (Instruction MMU Enable) in SR
    or1k_mtspr(OR1K_SPR_SYS_SR_ADDR, sr);
}

// Function to configure the IMMU
void configure_immu() {
    unsigned int immucr_value;

    // Step 1: Set the Page Table Base Pointer (PTBP) and flush ITLB
    immucr_value = ((PAGE_TABLE_BASE >> 10) << 10) | (1 << 0); // PTBP | ITF=1 (flush ITLB)
    or1k_mtspr(OR1K_SPR_IMMUCR_ADDR, immucr_value);

    // Step 2: Small delay to allow flush to propagate
    for (volatile int i = 0; i < 1000; i++);

    // Step 3: Clear ITF bit (flush bit should not remain set)
    immucr_value &= ~(1 << 0);
    or1k_mtspr(OR1K_SPR_IMMUCR_ADDR, immucr_value);
}

void flush_itlb() {
    uint32_t immucr = or1k_mfspr(OR1K_SPR_IMMU_IMMUCR_ADDR);
    immucr = OR1K_SPR_IMMU_IMMUCR_ITF_SET(immucr, 1); // Flush ITLB
    or1k_mtspr(OR1K_SPR_IMMU_IMMUCR_ADDR, immucr);
}

void set_execution_permissions() {
    uint32_t immupr = or1k_mfspr(OR1K_SPR_IMMU_IMMUPR_ADDR);
    immupr = OR1K_SPR_IMMU_IMMUPR_SXE1_SET(immupr, 1);  // Enable supervisor execution for region 1
    or1k_mtspr(OR1K_SPR_IMMU_IMMUPR_ADDR, immupr);
}

void invalidate_tlb_entry(uint32_t virt_addr) {
    or1k_mtspr(OR1K_SPR_IMMU_ITLBEIR_ADDR, virt_addr); // Invalidate specific TLB entry
}

void write_tlb_entry(uint32_t virt_addr, uint32_t phys_addr, uint32_t index) {
    uint32_t match_entry = OR1K_SPR_IMMU_ITLBW_MR_V_SET(0, 1) |
                           OR1K_SPR_IMMU_ITLBW_MR_VPN_SET(0, virt_addr >> 13);
    uint32_t trans_entry = OR1K_SPR_IMMU_ITLBW_TR_PPN_SET(0, phys_addr >> 13) |
                           OR1K_SPR_IMMU_ITLBW_TR_CI_SET(0, 0); // Enable caching

    or1k_mtspr(OR1K_SPR_IMMU_ITLBW_MR_ADDR(0, index), match_entry);
    or1k_mtspr(OR1K_SPR_IMMU_ITLBW_TR_ADDR(0, index), trans_entry);
}

void test_translation() {
    volatile int *ptr = (int *)TEST_VIRT_ADDR;
    *ptr = 0x41414141;  // Write to virtual address
    int value = *ptr;  // Read back value to verify translation
}

int main() {
    clear_itlb();
    //enable_immu();  // Step 1: Enable IMMU
    flush_itlb();  // Step 2: Flush ITLB
    set_execution_permissions();  // Step 3: Set execution permissions
    ////invalidate_tlb_entry(TEST_VIRT_ADDR);  // Step 4: Invalidate old TLB entry
    write_tlb_entry(TEST_VIRT_ADDR, TEST_PHYS_ADDR, TEST_TLB_INDEX);  // Step 5: Insert new TLB entry
    test_translation();  // Step 6: Test virtual-to-physical translation
    return 0;
}