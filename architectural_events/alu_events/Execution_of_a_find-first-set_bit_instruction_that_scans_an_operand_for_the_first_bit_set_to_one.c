int main(void)
{
    int pos = 0;
    // Use volatile to prevent compiler optimizations that might eliminate ffs calls
    volatile int x;
    for (int i = 0; i < 32; i++) {
        x = 1 << i;
        // __builtin_ffs with volatile input to ensure actual instruction emitted
        pos += __builtin_ffs(x);
    }
    // Return sum to prevent compiler optimizing away the calls
    return pos == 528 ? 0 : 1; // sum of 1..32 = 528
}