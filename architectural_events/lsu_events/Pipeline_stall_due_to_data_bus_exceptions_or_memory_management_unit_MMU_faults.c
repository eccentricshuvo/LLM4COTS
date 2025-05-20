int main(void)
{
    /* Attempt misaligned access to provoke dbus_stall.
     * Accessing a uint32_t pointer at an odd byte address causes alignment fault.
     * Use pure C load/store with misaligned pointer to trigger event as assembler mnemonics like lw are rejected.
     */

    volatile unsigned char mem[8] = {0,1,2,3,4,5,6,7};
    volatile unsigned int val;
    /* Misaligned address: &mem[1], which is not word-aligned */
    unsigned int *misaligned_ptr = (unsigned int *)(mem + 1);

    /* Pure C load from misaligned address to trigger dbus_stall */
    val = *misaligned_ptr;

    (void)val; /* Prevent optimization */

    return 0;
}