	.file	"Addition_with_carry_instruction_decoding.c"
	.section	.text
	.comm	result,4,4
	.align 4
	.global	main
	.type	main, @function
main:
	l.addi	r1, r1, -8
	l.sw	0(r1), r2
	l.addi	r2, r1, 8
	l.sw	4(r1), r9
#  6 "architectural_events/decode_events/Addition_with_carry_instruction_decoding.c" 1
	l.ori r3, r0, 0xFFFFFFFF
	l.ori r4, r0, 1
	l.add r5, r3, r4
	l.ori r6, r0, 2
	l.addc r7, r6, r0
	l.mov r19, r7
	
#  0 "" 2
	l.movhi	r17, ha(result)
	l.sw	lo(result)(r17), r19
	l.movhi	r17, hi(0)
	l.or	r11, r17, r17
	l.lwz	r2, 0(r1)
	l.lwz	r9, 4(r1)
	l.addi	r1, r1, 8
	l.jr	r9
	 l.nop

	.size	main, .-main
	.ident	"GCC: (GNU) 9.1.1 20190503"
