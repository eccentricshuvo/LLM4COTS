int main(void)
{
    // Use the debug-related instruction 'l.bpt' to trigger debug unit pipeline restart
    // Encoding for l.bpt (breakpoint) instruction: 0x14000000 (hypothetical)
    __asm volatile (".long 0x14000000\n\t");
    return 0;
}