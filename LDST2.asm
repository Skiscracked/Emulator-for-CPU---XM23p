;
; Example of direct addressing #2
; Load and store
;
; V2 <- V1 + 2
;
	data
	org	#100
V1	word	#FFFD
V2	word	#0
;
	code
	org	#200
LDST2
;
; Get address of V1 into R0
;
	movl	V1,R0		; R0 = #??00
	movh	V1,R0		; R0 = #0100
;
; R1 <- mem[R0]
;
; R0 contains the effective address
; mem[R0] is rvalue
; R1 is lvalue
;
; LD from-memory (rvalue),to-register (lvalue)
;
	LD	R0,R1		; R1 <- #FFFD
;
	ADD	#2,R1		; R1 <- R1 + 2
;
; Store in V2:
;
; Get address of V2 into R0
;
	movl	V2,R0		; R0 = #??00
	movh	V2,R0		; R0 = #0102
;
; R0 contains the effective address 
; mem[R0] <- R1
; 
	ST	R1,R0
;
; Put breakpoint on next address
;
BrkPtHere
;
	end LDST2
