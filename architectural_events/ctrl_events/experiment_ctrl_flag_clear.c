int main() {
    // Enable flags in SPR_SR (status register)
    //asm volatile (
    //    "l.mfspr r3, r0, 17\n\t"     // Read SPR_SR (SPR 17) into r3
    //    "l.ori   r3, r3, 0x10\n\t"   // Set flag enable bit (F bit)
    //    "l.mtspr r0, r3, 17\n\t"     // Write back to SPR_SR
    //);

    // Perform a flag-setting ALU operation (e.g., subtraction)
    // r4 = 5, r5 = 3 → result != 0, so Zero flag is NOT set → triggers flag_clear
    asm volatile (
        "l.addi  r4, r0, 5\n\t"      // r4 = 5
        "l.addi  r5, r0, 3\n\t"      // r5 = 3
        "l.sub   r6, r4, r5\n\t"     // r6 = r4 - r5 = 2 (does not set Zero flag)
    );

    return 0;
}

