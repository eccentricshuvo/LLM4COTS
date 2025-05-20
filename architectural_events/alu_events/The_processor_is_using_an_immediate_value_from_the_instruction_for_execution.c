int main(void)
{
    /* Use arithmetic immediate to trigger imm16_i */
    volatile int x = 0;
    x += 10;    /* This should compile to addi or similar immediate instruction */

    /* Use load with immediate offset to trigger imm16_i */
    volatile int arr[4] = {1, 2, 3, 4};
    volatile int y = arr[2]; /* This access uses an immediate offset in the load */

    /* Use branch with immediate offset - typically via C control flow */
    if (x != 0) {
        y = 1;
    } else {
        y = 0;
    }

    /* Use move to special purpose register with immediate via minimal asm */
    /* The 'mtspr' instruction uses specific SPR numbers, encoded in instruction.
       On OpenRISC 1000 GAS syntax, 'mtspr' is encoded as: mtspr spr, rN
       We must use correct SPR name or number.
       Use numerical spr number for register 1: r1 = spr number 1? 
       Usually SPR numbers are larger; basic example uses SPR 0x01 (corectrl) or 0x17 (sr).
       We can move immediate to r3 then mtspr to SPR 1 (example) by encoding spr number directly.
    */

    /* We'll encode using .word with encoded mtspr instruction as last resort. 
       But before that, try correct syntax mtspr <sprnum>, <rN> 
    */

    /* Assuming r1 as spr number 1; correct syntax from header is: mtspr <sprnum>, rN */
    /* Use register r3 containing immediate, spr number 1: */

    asm volatile(
        "l.addi r3, r0, 42\n\t"        /* Load immediate 42 into r3 (r0 is zero) */
        "mtspr 1, r3\n\t"              /* Move r3 to SPR 1 */
    );

    return 0;
}