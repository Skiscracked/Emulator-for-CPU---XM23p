; Sample Assembly Program to Test the XM23p Pipeline
; Author: Semilore Kayode
; Test of BEQ and BNE
; Date: 05/07/2024
;

;
PC    equ    R7
SP    equ    R6
LR    equ    R5
NUL   equ    '\0'
;
; Code segment
;
    Code
    org    #1000

Start
    movlz    #0,R0    ; Initialize R0 to 0
    movlz    #1,R1    ; Initialize R1 to 1

TestBNE
    cmp.b    R0,R1
    bne      NotEqual   ; Branch if R0 == R1
    movlz    #2,R2    ; This should not execute

TestBEQ
    cmp.b    R0,R0
    beq      Equal    ; Branch if R0 != R0
    movlz    #3,R3    ; This should not execute

Equal
    movlz    #4,R4    ; R4 = 4 if branch taken
    bra DONE

NotEqual
    movlz    #5,R5    ; R5 = 5 if branch taken

DONE
    bra DONE          ; Iniinte loop until control C
                      ; Because we don't want the emulator decoding 0 ~ BL

    end    Start
