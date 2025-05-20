int main() {
    int value = 10;
    int array[5] = {1, 2, 3, 4, 5};
    
    // Trigger address computation for load/store operations
    asm volatile("l.xor r4, r5, r6"); // l.xor instruction for triggering address computation
    
    return 0;
}