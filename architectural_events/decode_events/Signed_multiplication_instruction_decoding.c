int main() {
    int a = 5;
    int b = 3;
    int result;

    asm volatile (
        "l.mul %0, %1, %2"   // Signed multiplication of a and b, store result in result register
        : "=r" (result)
        : "r" (a), "r" (b)
    );
    
    return 0;
}