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


    swap R1,R2      ; R2 = R1 (0x02), R1 = R2 (0x03)
    sra R2          ; Before, R2 = 0x02. After R2 = 0x01
    rrc R3          ; Before R3 = 0x80(1000 0000), after R3 = 0x40(0100 0000)
    swpb R1         ; Before R1 = 0x02(0000 0010), after R1 = 0x0200(0000 0010 0000 0000)
    sxt R4          ; Before R4 = 0x80(1000 0000), after R4 = 0XFF80(1111 1111 1000 0000)

    end Main
