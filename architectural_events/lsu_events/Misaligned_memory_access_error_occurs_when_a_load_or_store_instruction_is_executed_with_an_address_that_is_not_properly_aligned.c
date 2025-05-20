int main(void)
{
    volatile char data[8];
    volatile char *ptr = data;

    // Create a deliberately misaligned address: add 1 to the base address to misalign
    volatile char *misaligned_ptr = (volatile char *)((char *)ptr + 1);

    // Attempt to perform a 4-byte load (int) from a misaligned address
    // This should trigger align_err_word due to misalignment (address not multiple of 4)
    volatile int value = *(volatile int *)misaligned_ptr;

    // Avoid compiler optimizations
    (void)value;

    return 0;
}