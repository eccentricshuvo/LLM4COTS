	.file	"Pipeline_stall_due_to_delay_slot_instruction.c"
	.section	.text
	.align 4
	.global	main
	.type	main, @function
main:
	l.addi	r1, r1, -16
	l.sw	8(r1), r2
	l.addi	r2, r1, 16
	l.sw	12(r1), r9
	l.sw	-12(r2), r0
	l.sw	-16(r2), r0
	l.lwz	r17, -12(r2)
#  17 "architectural_events/lsu_events/Pipeline_stall_due_to_delay_slot_instruction.c" 1
	l.add r3, r17, r0      
	1:                    
	l.addi r4, r0, 10     
	l.sub r5, r4, r3      
	l.sfeq r5, r0	       
	l.bf 2f               
	l.addi r3, r3, 1      
	l.j 1b                
	2:                    
	l.add r17, r3, r0      
	
#  0 "" 2
	l.sw	-12(r2), r17
	l.lwz	r17, -12(r2)
	l.sw	-16(r2), r17
	l.lwz	r17, -16(r2)
	l.or	r11, r17, r17
	l.lwz	r2, 8(r1)
	l.lwz	r9, 12(r1)
	l.addi	r1, r1, 16
	l.jr	r9
	 l.nop

	.size	main, .-main
	.ident	"GCC: (GNU) 9.1.1 20190503"
