int main(void)
{
    volatile int r1 = 10, r2 = 20, r3 = 5, r4 = 30, r5 = 15, r6 = 3;
    volatile int result = 0;

    // ADD r1, r2, r3  (r1 = r2 + r3)
    result = r2 + r3;

    // SUB r4, r5, r6  (r4 = r5 - r6)
    result = r5 - r6;

    // Branch with immediate destination simulation:
    // Using volatile to simulate operand selection with immediate (no real branch to avoid host-I/O)
    // Just simulate the effect of selecting immediate operand in arithmetic calculation
    volatile int branch_imm = 100;
    result = r1 + branch_imm;

    // Another arithmetic with immediate operand
    result = r3 + 7;

    // Avoid optimization away
    return (int)result;
}