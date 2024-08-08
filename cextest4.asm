
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
	cex	eq,$0,$2
	add	R2,R1
	sub	$2,R1
	movls	#02,R3
	bra	Done


Done
	bra	Done
	end	Main
	
	