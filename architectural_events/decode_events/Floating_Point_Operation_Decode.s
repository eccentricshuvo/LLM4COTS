	.file	"Floating_Point_Operation_Decode.c"
	.section	.text
	.align 4
	.global	trigger_floating_point_operation_decode
	.type	trigger_floating_point_operation_decode, @function
trigger_floating_point_operation_decode:
	l.addi	r1, r1, -8
	l.sw	0(r1), r2
	l.addi	r2, r1, 8
	l.sw	4(r1), r9
#  4 "architectural_events/decode_events/Floating_Point_Operation_Decode.c" 1
	l.nop
	l.nop
	.word 0xE2000000
	l.nop
	
#  0 "" 2
	l.nop
	l.lwz	r2, 0(r1)
	l.lwz	r9, 4(r1)
	l.addi	r1, r1, 8
	l.jr	r9
	 l.nop

	.size	trigger_floating_point_operation_decode, .-trigger_floating_point_operation_decode
	.align 4
	.global	main
	.type	main, @function
main:
	l.addi	r1, r1, -8
	l.sw	0(r1), r2
	l.addi	r2, r1, 8
	l.sw	4(r1), r9
	l.jal	trigger_floating_point_operation_decode
	 l.nop

	l.movhi	r17, hi(0)
	l.or	r11, r17, r17
	l.lwz	r2, 0(r1)
	l.lwz	r9, 4(r1)
	l.addi	r1, r1, 8
	l.jr	r9
	 l.nop

	.size	main, .-main
	.ident	"GCC: (GNU) 9.1.1 20190503"
