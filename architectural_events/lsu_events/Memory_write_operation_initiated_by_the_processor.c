int main(void)
{
    volatile int array[2] = {0, 0};
    volatile int value = 42;

    /* Store operations to provoke DBUS_WE signal */
    array[0] = value;    // word store
    ((volatile char *)array)[1] = 0xFF;  // byte store at unaligned byte offset
    ((volatile short *)array)[1] = 0x1234; // halfword store

    /* Load operation to simulate normal memory access */
    volatile int temp = array[0];
    (void)temp;

    return 0;
}