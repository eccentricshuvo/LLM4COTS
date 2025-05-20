int main() {
    asm volatile (
        "l.mfspr r3, r4, 67"   // Move from Special Purpose Register r3, r4, 67
    );
    return 0;
}