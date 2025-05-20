int main(void)
{
    volatile int a = 1234;
    volatile int b = 5678;
    volatile int c = 0;

    // Trigger multiplication operation and completion of mul_result_o
    // Using pure C multiplication, relying on compiler to emit mul instruction
    c = a * b;

    // Use c to prevent optimization
    if (c == 0)
        return 1;

    return 0;
}