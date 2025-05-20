int main(void)
{
    /*
     * Triggering signed multiplication overflow in 32-bit int:
     * Multiplying two large positive numbers produces a negative result due to overflow.
     * Example: INT_MAX * 2 overflows.
     */
    int a = 0x40000000;  // 2^30, large positive
    int b = 4;           // multiplying by 4 yields 2^32 which overflows int32_t
    volatile int c;

    c = a * b;           // signed multiplication overflow

    return c;            // return to avoid optimization away
}