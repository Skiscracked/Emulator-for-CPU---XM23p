; Sample Assembly Program to Test the XM23p Pipeline
; Author: Semilore Kayode
; Date: 29/06/2024

    code
    org #1000

; Initialize registers
;
Main 
    movl #1,R0     ; Load 0x01 into R0    
    movl #2,R1     ; Load 0x02 into R1
    movl #3,R2     ; Load 0x03 into R2  
    movl #80,R3     ; Load 0x04 into R3   
    movl #80,R4     ; Load 0x05 into R4

    ; Arithmetic operations
    add.b R0,R1           ; R1 = R1 + R0 (0x02 + 0x01 = 0x03)
    sub.b R1,R2           
    addc.b R3,R4          ; R4 = R4 + R3 + Carry (0x80 + 0x80 + 1 = 0x00)
    subc.b R1,R2          ; R2 = R2 - R3 - Carry 

    end Main
