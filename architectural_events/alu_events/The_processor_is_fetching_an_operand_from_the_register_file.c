int main(void)
{
    volatile int a = 5;
    volatile int b = 10;
    volatile int c = 0;
    volatile int d = 15;
    volatile int e = 20;

    /* Arithmetic operations: ADD, SUB */
    c = a + b;
    c = d - a;

    /* Mimic MUL (no direct C operator for explicit MUL, but * is multiply) */
    c = b * e;

    /* Load/store simulation: read/write from/to volatile variables */
    e = c;
    a = e;

    /* Control-flow branch: simple if to emulate BEQ/BNE usage */
    if (a == b)
        c = d;
    else
        c = e;

    /* Use OpenRISC inline assembly encoding for instructions to ensure operand fetching */
    asm volatile (".word 0x08430405"); /* add r3,r4,r5 */
    asm volatile (".word 0x08450608"); /* sub r6,r7,r8 */
    asm volatile (".word 0x08490a0b"); /* mul r9,r10,r11 */
    asm volatile (".word 0x900c0d00"); /* lwz r12,0(r13) */
    asm volatile (".word 0x901e0f00"); /* stw r14,0(r15) */
    asm volatile (".word 0xe1100000"); /* beq r16,r17,label */

label:
    return (int)c;
}