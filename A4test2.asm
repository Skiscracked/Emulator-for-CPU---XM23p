; Sample Assembly Program to Test the XM23p Pipeline
; Author: Semilore Kayode
; Test of BC and BNC
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
    org    #2000

Start
    movlz    #FF,R0    ; Initialize R0 to 255
    add.b    #1,R0        ; R0 should now be 0 with Carry

TestBC
    bc    CarrySet    ; Branch if Carry is set
    movlz    #2,R2    ; This should not execute

TestBNC
    bnc    NoCarry    ; Branch if No Carry
    movlz    #3,R3    ; This should not execute

CarrySet
    movlz    #4,R4    ; R4 = 4 if branch taken
    bra      DONE

NoCarry
    movlz    #5,R5    ; R5 = 5 if branch taken

DONE
    bra    DONE
    end    Start
