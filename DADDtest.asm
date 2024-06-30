A	equ	#7FFF
B	equ	#1
; Code	
	org #1000
Main
	movl	A,R1
	movh	A,R1
	movl	B,R2
	movh	B,R2
	add	R1,R2
	end	Main
; R2 should contain 2041
