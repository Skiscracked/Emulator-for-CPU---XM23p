
;
; Sum an array of 16-bit numbers
; ECED 3403
; 15 May 24
;
	CODE
	org	#1000
;
; Initialize registers
;
Main	movlz	Array,R0	; r0=Address of the array
	ld	R0,R3		; load stopper into r3
	add	#2,R0		; Move R0 to the next element (first actual data element) - incremented
	mov	R1,R2		; setting r2 as sum register and making it 0
;
loop	ld	R0,R1		; load the array's element into r1
	add	R1,R2		; Add the element to the sum
	add	#2,R0		; Increment R0 to point to the next element in the array
;
; Check if we have reached the limit
;
	sub	#1,R3		; stopper - 1
	bz	Done		; end loop if stopper is 0
;
	bra	loop		; continue adding
;
; adding complete, result in R2
;
Done
;
; Completed
;
BraWait	bra	BraWait
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Data space
;
	DATA
	org	#40
;
; Array 
;
Array	word	#0
	word	#1
	word	#1
	word	#1
    word	#1
    word	#1
;
; Storage for the sum result
;
;
	end	Main