int main(void)
{
    // Use a known debug-unit-access instruction: mtspr to write to debug register 20.
    // Encoding mtspr r0, r0, 20 as inline .word:
    // Using .word 0x7c001214 for mtspr r0, r0, 20 (move to SPR 20)
    asm volatile(".word 0x7c001214"); // mtspr r0, r0, 20: trigger debug unit access

    // Data-dependent load-use to cause pipeline stalls more likely
    volatile int data = 0;
    volatile int *ptr = &data;
    int x;
    x = *ptr;   // load
    x += 1;     // use loaded value immediately

    return x;
}