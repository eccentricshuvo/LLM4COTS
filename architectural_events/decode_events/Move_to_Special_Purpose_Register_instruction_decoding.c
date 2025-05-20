int main() {
    int data = 10;

    asm volatile (
        "l.mtspr r3, %0, 0"
        :
        : "r" (data)
    );
    
    return 0;
}