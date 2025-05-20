int main(void)
{
    volatile int cond = 1;
    volatile int result = 0;
    volatile int reg1 = 5;
    volatile int reg2 = 3;
    volatile int imm = 7;

    if (cond)
    {
        // Branch instruction: unconditional branch simulated by if
        // This causes decode_b to select branch destination operand
        result = reg1 + reg2;
    }
    else
    {
        // Arithmetic with immediate operand: decode_b selects immediate
        result = reg1 + imm;
    }

    // Arithmetic with register operands: decode_b selects register values
    result += reg1 - reg2;

    // Use volatile to prevent optimization
    return result;
}