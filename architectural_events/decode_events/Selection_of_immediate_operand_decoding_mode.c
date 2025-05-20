int main(void) {
    int result;
    
    asm volatile (
        "l.movhi %0, 0xABCD"
        : "=r" (result)
    );
    
    return 0;
}