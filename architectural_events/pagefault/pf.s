.section .text
.global _start
.global main

_start:
    l.jal main
    l.nop

main:
    # Enable MMU (If Required)
    l.mfspr r3, r0, 17       # Read SR (Supervision Register)
    l.ori   r3, r3, 0xC      # Enable IMMU & DMMU (IME=1, DME=1)
    l.mtspr r0, r3, 17       # Write back to SR

    # Load an unmapped address
    l.movhi r4, 0xDEAD
    l.ori   r4, r4, 0xBEEF  # Address = 0xDEADBEEF

    # Attempt to access unmapped memory (this triggers a page fault)
    l.lwz r5, 0(r4)        # Load from invalid address

    # If execution reaches here, no page fault occurred (unexpected)
    l.j exit
    l.nop

.exception_handler:
    # Read exception-related registers
    l.mfspr r10, r0, 18     # Read PPC (Previous PC)
    l.mfspr r11, r0, 48     # Read EEAR (Faulting Address)

    # Halt execution
    l.j exit
    l.nop

exit:
    # Proper simulation exit
    l.nop
    l.nop
    l.j exit
    l.nop

