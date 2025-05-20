int main(void)
{
    int r1 = 42;
    int r2 = 42;
    volatile int flag = 0;

    /* Use OpenRISC instructions to compare and set flag */
    asm volatile(
        "sub r3, %1, %2    \n\t" /* r3 = r1 - r2; sets flags including zero */
        "mov r4, 0         \n\t" /* r4 = 0 */
        "mov r5, 1         \n\t" /* r5 = 1 */
        "beq label_equal   \n\t" /* branch if zero flag set (r1 == r2) */
        "mov %0, r4        \n\t" /* flag = 0 */
        "b label_done      \n\t"
        "label_equal:      \n\t"
        "mov %0, r5        \n\t" /* flag = 1 */
        "label_done:       \n\t"
        : "=r"(flag)
        : "r"(r1), "r"(r2)
        : "r3", "r4", "r5", "memory"
    );

    return flag; /* return 1 if equal, 0 otherwise */
}