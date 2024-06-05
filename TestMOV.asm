;
; Test MOV instructions
; ECED 3403
; Date: Today
;

    CODE
    org #1000

; Initialize registers
;
Main
    movlz  #10,R0  ; 
    movls  #20,R1  ; 
    movh   #30,R2  ;
    movl   #40,R3  ; 

; Completed
;
BraWait
    bra BraWait        ; Infinite loop to halt the program

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Data space
;
    DATA
    org #40


    ; Define any data required by the program (none needed for this test)

    end Main
