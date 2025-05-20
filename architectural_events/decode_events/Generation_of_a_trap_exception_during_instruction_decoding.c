int main() {
    asm volatile (
        ".word 0x00000018"  // Encoding that triggers a trap exception
    );
    return 0;
}