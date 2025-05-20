int main() {
    int a = 12345;
    int b = 6789;

    // Case 1: Signed Multiply — triggers ALU multiply
    volatile int result_signed = a * b;

    // Case 2: Unsigned Multiply — force values to unsigned
    volatile unsigned int ua = (unsigned int)a;
    volatile unsigned int ub = (unsigned int)b;
    volatile unsigned int result_unsigned = ua * ub;

    // Case 3: Trigger more pipeline activity — consecutive multiplies
    volatile int result2 = a * 2;
    volatile int result3 = b * 4;

    return 0;
}
