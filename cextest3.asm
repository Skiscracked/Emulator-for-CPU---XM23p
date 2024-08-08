
V1	equ	$10
V2	equ	$7
LR	equ	R5
PC	equ	R7

	CODE
	org	#1000
Main
	movlz	V1,R1
	movlz	V2,R2
	setcc	Z
	cex	eq,$2,$2
	bl	Subr
	add	R2,R1
	sub	$2,R1
	movls	#02,R3
	bra	Done

Subr
	movls	#0F,R4
	mov	LR,PC
	movlz	#FF,R0	; should not execute

Done
	bra	Done
	end	Main
	
	