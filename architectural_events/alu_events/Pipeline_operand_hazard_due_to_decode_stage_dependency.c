int main(void)
{
    volatile int a = 0, b = 1;

    /* Trigger pipeline operand hazard due to decode stage dependency.
       Sequence:
         a = b + 1;   // arithmetic instruction depending on b
         b = a + 2;   // arithmetic instruction depending on updated a
       This dependency chain forces the decode stage to resolve operands and may cause stalls.
    */
    a = b + 1;
    b = a + 2;

    /* Branch dependent on computed values, creating branch destination calculations.
       This introduces decode stage dependencies involving branch instructions.
    */
    if (b == 4) {
        a = b + a;
    } else {
        b = a + b;
    }

    /* Load-store dependency:
       Emulated by volatile pointers and accesses forcing compiler to generate load/store pairs
       with dependencies on same registers.
    */
    volatile int array[2] = {0, 0};
    array[0] = a;
    b = array[0];

    /* Return something to avoid optimization away */
    return a + b;
}