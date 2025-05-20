.section .text
.global _start
.global main

# SPR Index Values (Directly from Manual)
.equ SPR_EVBAR, 0x11   # Exception Vector Base Address Register
.equ SPR_SR, 0x17      # Supervision Register
.equ SPR_EEAR, 0x30    # Effective Address Register (Exception)
.equ SPR_EPCR, 0x20    # Exception Program Counter Register

# Entry Point
_start:
    l.j main          # Jump to main function
    l.nop

# Main Function (Proper Entry Point)
main:
    # Step 1: Load exception_handler address properly
    l.movhi r3, HI(exception_handler)  # Load high part of address
    l.ori   r3, r3, LO(exception_handler)  # Load low part of address
    l.mtspr r0, r3, SPR_EVBAR   # Set Exception Vector Base Address Register

    # Step 2: Enable MMU and exception handling
    l.mfspr r3, r0, SPR_SR      # Read Supervision Register (SR)
    l.ori   r3, r3, 0x00000040  # Enable IMMU (Instruction MMU)
    l.mtspr r0, r3, SPR_SR      # Write back to enable MMU

    # Step 3: Trigger an Instruction Page Fault
    l.movhi r4, 0xDEAD          # Load invalid address (non-mapped region)
    l.ori   r4, r4, 0xBEEF      # Fully define an invalid memory location
    l.jalr  r4                 # Jump to the invalid address (triggers page fault)
    l.nop                       # Delay slot (executed before exception takes effect)

    # Step 4: If no fault occurred, exit normally
    l.j exit_program            # If page fault doesn’t occur (unexpected), exit safely
    l.nop

# Step 5: Exception Handler
exception_handler:
    l.mfspr r5, r0, SPR_EEAR    # Read Effective Address Register (fault address)
    l.mfspr r6, r0, SPR_EPCR    # Read Program Counter at the time of fault

    # Return from exception (recover if possible)
    l.mfspr r3, r0, SPR_SR      # Restore SR
    l.mtspr r0, r3, SPR_SR
    l.rfe                        # Return from exception

# Step 6: Exit Program Gracefully
exit_program:
    l.nop
    l.nop
    l.nop
    l.nop
    l.jr r9                     # Return (if in a caller context)
    l.nop

