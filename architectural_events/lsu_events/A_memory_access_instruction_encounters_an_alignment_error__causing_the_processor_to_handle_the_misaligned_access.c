int main(void)
{
    // Use a packed struct to enforce unaligned access to provoke align_err
    struct __attribute__((packed)) Misaligned {
        char c;
        int w;
    } m = {0, 0x12345678};

    // Access the int field via a misaligned pointer
    volatile int *p = (volatile int *)((char *)&m + 1);

    // Dereference to provoke misaligned load (load word at address not multiple of 4)
    volatile int val = *p;

    (void)val; // Prevent optimization away

    return 0;
}