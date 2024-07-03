; Sample Assembly Program to Test the XM23p Pipeline
; Author: Semilore Kayode
; Date: 02/07/2024

    code
    org #1000

; Initialize registers
;
Main 
    movl #FF,R0     ; Load 0xFF into LOW R0    
    movh #FF00,R0     ; Load 0xFF into HIGH R0
    movl #1,R1      ; Load 0x01 into LOW R1
    movh #0,R1     ; Load 0x00 into HIGH R1


    add.b R1,R0     ; R0 = FF 00, Carry = 1

    end Main
