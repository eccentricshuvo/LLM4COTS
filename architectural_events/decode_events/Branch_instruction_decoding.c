int main() {
    volatile int x = 1;
    volatile int y = 0;
    if (x) {
        y = 1;
    }
    if (y) {
        x = 0;
    }
    __asm__ __volatile__("l.bf 0" : : );
    return 0;
}