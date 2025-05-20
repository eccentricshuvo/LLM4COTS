.global _start
.section .text

_start:
    # Step 1: Set up stack pointer (if needed)
    l.movhi r1, 0x8000  # Set stack to 0x80000000 (example)
    l.ori   r1, r1, 0x0000

    # Step 2: Enable IMMU and DMMU
    l.mtspr r0, r0, 0x1800  # Enable Instruction MMU
    l.mtspr r0, r0, 0x2800  # Enable Data MMU

    # Step 3: Set up a TLB entry (Virtual: 0x400012000 -> Physical: 0x30012000)
    l.mtspr r3, r3, 0x1820  # Virtual Page Number in ITLB
    l.mtspr r3, r3, 0x1821  # Physical Page Number in ITLB

    l.mtspr r6, r6, 0x2820  # Virtual Page Number in DTLB
    l.mtspr r6, r6, 0x2821  # Physical Page Number in DTLB

    # Step 4: Perform a memory access to trigger translation
    l.movhi r4, 0x4000  # Load upper part of virtual address
    l.ori   r4, r4, 0x12000  # Load lower part, r4 = 0x400012000

    l.lwz   r3, 0(r4)  # Load word from virtual address

    # Step 5: Loop infinitely (avoid undefined execution)
loop:
    l.j loop
    l.nop

