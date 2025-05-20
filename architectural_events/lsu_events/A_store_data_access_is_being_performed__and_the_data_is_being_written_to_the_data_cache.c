int main(void)
{
    volatile int arr[4] = {0};
    volatile int value = 0xDEADBEEF;

    // Use explicit store assembly with correct OpenRISC mnemonic 'stw' (Store Word)
    // corrected mnemonic from ISA document is 'stw' not 'sw'
    for (int i = 0; i < 4; i++)
    {
        int val = value + i;
        __asm__ volatile (
            "stw %1, 0(%0) \n\t"  // store word, id=0x7c00000b encoding
            :
            : "r" (&arr[i]), "r" (val)
            : "memory"
        );
    }

    // Dummy read to avoid optimizing away
    volatile int dummy = arr[0] + arr[1] + arr[2] + arr[3];
    (void)dummy; // prevent unused variable warning

    return 0;
}