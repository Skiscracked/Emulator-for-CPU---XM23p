;
; Test of BLT and BRA
;
PC    equ    R7
SP    equ    R6
LR    equ    R5
NUL   equ    '\0'
;
; Code segment
;
    Code
    org    #4000

Start
    movlz    #1,R0    ; Initialize R0 to 1
    movlz    #2,R1    ; Initialize R1 to 2

TestBLT
    cmp.b    R1,R0
    blt    LessThan    ; Branch if R1 < R0
    movlz    #2,R2    ; This should not execute

AlwaysBranch
    bra    Always    ; Always branch
    movlz    #3,R3    ; This should not execute

LessThan
    movlz    #4,R4    ; R4 = 4 if branch taken
    bra DONE

Always
    movlz    #5,R5    ; R5 = 5 if branch taken

DONE
    bra    DONE
    end    Start
