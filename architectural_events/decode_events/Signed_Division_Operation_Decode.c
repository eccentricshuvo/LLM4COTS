int signed_division_operation_decode() {
    volatile int a = 10;
    volatile int b = 2;
    volatile int c = a / b;
    return c;
}

int main() {
    signed_division_operation_decode();
    return 0;
}