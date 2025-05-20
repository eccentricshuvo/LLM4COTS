	.file	"prologue.c"
	.section	.text
	.align 4
	.global	main
	.type	main, @function
main:
	l.addi	r1, r1, -8
	l.sw	0(r1), r2
	l.addi	r2, r1, 8
	l.sw	4(r1), r9
	l.movhi	r17, hi(0)
	l.or	r11, r17, r17
	l.lwz	r2, 0(r1)
	l.lwz	r9, 4(r1)
	l.addi	r1, r1, 8
	l.jr	r9
	 l.nop

	.size	main, .-main
	.ident	"GCC: (GNU) 9.1.1 20190503"
