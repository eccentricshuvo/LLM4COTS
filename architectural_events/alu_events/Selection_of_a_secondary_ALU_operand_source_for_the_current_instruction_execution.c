int main(void)
{
    volatile int a = 5;
    volatile int b = 10;
    volatile int c = 0;
    volatile int d = 15;
    volatile int res = 0;

    /* ALU operation with immediate operand: triggers alu_secondary selection of immediate */
    res = a + 7;

    /* ALU operation with two registers: triggers alu_secondary selection of register */
    res = b + d;

    /* Additional mix to reinforce the event */
    c = res - 3;
    res = c & b;

    return (int)res;
}