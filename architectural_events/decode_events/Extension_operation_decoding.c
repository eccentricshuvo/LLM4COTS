int main(void) {
    asm volatile (
        ".word 0x38000000"
    );
    return 0;
}