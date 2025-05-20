int main() {
    asm volatile (
        "l.j 0x0000000A"   // Jump instruction to trigger instruction decoding for jump targets
    );
    return 0;
}