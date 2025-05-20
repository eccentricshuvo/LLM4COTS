	.file	"Instruction_execution_triggers_decode_bubble_insertion.c"
	.section	.text
	.global	mem
	.section	.data
	.align 4
	.type	mem, @object
	.size	mem, 32
mem:
	.long	-1515870811
	.long	1515870810
	.long	-1
	.long	0
	.long	305419896
	.long	-889275714
	.long	-559038737
	.long	0
	.section	.text
	.align 4
	.global	main
	.type	main, @function
main:
	l.addi	r1, r1, -24
	l.sw	16(r1), r2
	l.addi	r2, r1, 24
	l.sw	20(r1), r9
	l.sw	-12(r2), r0
	l.j	.L2
	 l.nop

.L3:
	l.movhi	r17, ha(mem)
	l.lwz	r19, -12(r2)
	l.ori	r21, r0, 2
	l.sll	r19, r19, r21
	l.addi	r17, r17, lo(mem)
	l.add	r17, r19, r17
	l.lwz	r17, 0(r17)
	l.lwz	r17, -12(r2)
	l.addi	r17, r17, 1
	l.sw	-12(r2), r17
.L2:
	l.lwz	r19, -12(r2)
	l.ori	r17, r0, 7
	l.sfles	r19, r17
	l.bf	.L3
	 l.nop

	l.sw	-16(r2), r0
	l.sw	-20(r2), r0
	l.j	.L4
	 l.nop

.L5:
	l.lwz	r17, -20(r2)
	l.andi	r19, r17, 7
	l.movhi	r17, ha(mem)
	l.ori	r21, r0, 2
	l.sll	r19, r19, r21
	l.addi	r17, r17, lo(mem)
	l.add	r17, r19, r17
	l.lwz	r17, 0(r17)
	l.sw	-24(r2), r17
	l.lwz	r19, -24(r2)
	l.lwz	r17, -16(r2)
	l.add	r17, r19, r17
	l.sw	-24(r2), r17
	l.lwz	r17, -20(r2)
	l.addi	r17, r17, 3
	l.or	r19, r17, r17
	l.lwz	r17, -24(r2)
	l.mul	r17, r17, r19
	l.sw	-16(r2), r17
	l.lwz	r17, -20(r2)
	l.addi	r17, r17, 1
	l.sw	-20(r2), r17
.L4:
	l.lwz	r19, -20(r2)
	l.ori	r17, r0, 1023
	l.sfles	r19, r17
	l.bf	.L5
	 l.nop

	l.lwz	r17, -16(r2)
	l.or	r11, r17, r17
	l.lwz	r2, 16(r1)
	l.lwz	r9, 20(r1)
	l.addi	r1, r1, 24
	l.jr	r9
	 l.nop

	.size	main, .-main
	.ident	"GCC: (GNU) 9.1.1 20190503"
