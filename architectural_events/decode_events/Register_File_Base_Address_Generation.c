int main() {
    asm volatile (
        "l.addi r1, r0, 1"  // Use a different immediate value to store in r1 for Register File Base Address Generation
    );
    return 0;
}