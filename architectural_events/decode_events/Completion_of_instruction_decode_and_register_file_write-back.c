int main() {
    volatile int x = 5;
    x = x + 1;
    return x;
}