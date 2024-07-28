; Sample Assembly Program to Test the XM23p Pipeline
; Author: Semilore Kayode
; Test of BN and BGE
; Date: 05/07/2024
;
PC    equ    R7
SP    equ    R6
LR    equ    R5
NUL   equ    '\0'
;
; Code segment
;
    Code
    org    #3000

Start
    movlz    #80,R0    ; Initialize R0 to -128 (signed)

TestBN
    cmp.b    #0,R0
    bn    Negative    ; Branch if R0 < 0
    movlz    #2,R2    ; This should not execute

TestBGE
    movlz    #0,R1
    cmp.b    #0,R1
    bge    NotNegative    ; Branch if R1 >= 0

Negative
    movlz    #4,R4    ; R4 = 4 if branch taken
    bra      DONE

NotNegative
    movlz    #5,R5    ; R5 = 5 if branch taken

DONE
    bra    DONE
    end    Start