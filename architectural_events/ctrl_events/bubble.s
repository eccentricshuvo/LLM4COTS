	.file	"bubble.c"
	.section	.text
	.global	mem_word
	.section	.data
	.align 4
	.type	mem_word, @object
	.size	mem_word, 4
mem_word:
	.long	-559038737
	.section	.text
	.align 4
	.global	main
	.type	main, @function
main:
	l.addi	r1, r1, -20
	l.sw	12(r1), r2
	l.addi	r2, r1, 20
	l.sw	16(r1), r9
	l.sw	-12(r2), r0
	l.sw	-16(r2), r0
	l.j	.L2
	 l.nop

.L3:
	l.movhi	r17, ha(mem_word)
	l.lwz	r17, lo(mem_word)(r17)
	l.sw	-20(r2), r17
	l.lwz	r19, -12(r2)
	l.lwz	r17, -20(r2)
	l.add	r17, r19, r17
	l.sw	-12(r2), r17
	l.lwz	r17, -16(r2)
	l.addi	r17, r17, 1
	l.sw	-16(r2), r17
.L2:
	l.lwz	r19, -16(r2)
	l.ori	r17, r0, 49999
	l.sfles	r19, r17
	l.bf	.L3
	 l.nop

	l.lwz	r17, -12(r2)
	l.or	r11, r17, r17
	l.lwz	r2, 12(r1)
	l.lwz	r9, 16(r1)
	l.addi	r1, r1, 20
	l.jr	r9
	 l.nop

	.size	main, .-main
	.ident	"GCC: (GNU) 9.1.1 20190503"
