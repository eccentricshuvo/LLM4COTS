	.file	"Pipeline_stall_due_to_delay_slot_instruction_v2.c"
	.section	.text
	.align 4
	.global	main
	.type	main, @function
main:
	l.addi	r1, r1, -16
	l.sw	8(r1), r2
	l.addi	r2, r1, 16
	l.sw	12(r1), r9
	l.ori	r17, r0, 5
	l.sw	-12(r2), r17
	l.sw	-16(r2), r0
	l.j	.L2
	 l.nop

.L5:
	l.lwz	r17, -16(r2)
	l.addi	r17, r17, 1
	l.sw	-16(r2), r17
	l.lwz	r17, -12(r2)
	l.addi	r17, r17, -1
	l.sw	-12(r2), r17
	l.lwz	r19, -12(r2)
	l.ori	r17, r0, 1
	l.sflts	r19, r17
	l.bf	.L7
	 l.nop

	l.nop
.L2:
	l.lwz	r19, -12(r2)
	l.ori	r17, r0, 1
	l.sfges	r19, r17
	l.bf	.L5
	 l.nop

	l.j	.L4
	 l.nop

.L7:
	l.nop
.L4:
	l.lwz	r17, -16(r2)
	l.or	r11, r17, r17
	l.lwz	r2, 8(r1)
	l.lwz	r9, 12(r1)
	l.addi	r1, r1, 16
	l.jr	r9
	 l.nop

	.size	main, .-main
	.ident	"GCC: (GNU) 9.1.1 20190503"
