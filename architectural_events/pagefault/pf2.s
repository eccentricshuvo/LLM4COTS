.section .vectors, "ax"
.org 0x100
.global main
_start:
    l.movhi r4, hi(main)
    l.ori r4, r4, lo(main)
    l.jr r4
    l.nop

/* Exception Handler */
.org 0x200
.global _exception_handler
_exception_handler:
    l.mfspr r3, r0, 0x11  /* Read Exception PC (EPCR) */
    l.mfspr r4, r0, 0x40  /* Read Exception Cause (ESR) */
    l.mfspr r5, r0, 0x48  /* Read Faulting Address */

    /* Clear registers for safe return */
    l.addi r6, r0, 0x0  
    l.mtspr r0, r6, 0x48  /* Clear faulting address */
    l.mtspr r0, r4, 0x40  /* Restore cause register */

    l.rfe  /* Return from exception */
    l.nop

.section .text
.global main
main:
    /* Enable IMMU */
    l.mfspr r3, r0, 17  /* Read Supervision Register (SR) */
    l.ori r3, r3, 0x01  /* Enable IMMU */
    l.mtspr r0, r3, 17  /* Write back to SR */

    /* Enable User Mode */
    l.mfspr r4, r0, 1  /* Read Current Mode */
    l.ori r4, r4, 0x01  /* Set User Mode */
    l.mtspr r0, r4, 1  /* Write back */

    /* Load an invalid memory address (forcing page fault) */
    l.movhi r5, hi(0x80000000)  /* Load upper bits */
    l.ori   r5, r5, lo(0x80000000)  /* Load lower bits */

    /* Access invalid memory (should trigger page fault) */
    l.lwz r6, 0(r5)  /* Load from invalid page */

epilogue:
    /* Additional execution (should not be reached if fault works) */
    l.sw 0(r5), r6
    l.addi r1, r0, 0x1
    l.addi r2, r1, 0x2
    l.addi r3, r2, 0x4
    l.addi r4, r3, 0x8

    /* Finish execution */
    l.movhi r3, 0x0
    l.nop 0x1
    l.nop
    l.nop

