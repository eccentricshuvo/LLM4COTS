int main() {
    // Triggering a DTLB miss exception handling by accessing a memory location
    volatile int* addr = (int*)0x1000;  // Virtual address that will cause a DTLB miss
    int value;

    // Perform a memory access to trigger the DTLB miss exception
    asm volatile(
        ".word 0x00004133"  // l.lwz %r1, 0(%r2)
        : "=r" (value)
        : "r" (addr)
    );

    return 0;
}