; Sample Assembly Program to Test the XM23p Pipeline
; Author: Semilore Kayode
; Date: 29/06/2024

    code
    org #1000

; Initialize registers
;
Main
    movl #1,R0     
    movh #1,R0     ; Load 0x01 into R0
    movl #2,R1     
    movh #2,R1     ; Load 0x02 into R1
    movl #3,R2     
    movh #3,R2     ; Load 0x03 into R2
    movl #4,R3     
    movh #4,R3     ; Load 0x04 into R3
    movl #5,R4     
    movh #5,R4     ; Load 0x05 into R4

    ; Arithmetic operations
    add R0,R1           ; R1 = R1 + R0 (0x02 + 0x01 = 0x03)
    sub R1,R2           ; R1 = R1 - R2 (0x03 - 0x03 = 0x00)
    addc R2,R3          ; R2 = R2 + R3 + Carry (0x03 + 0x04 + 0 = 0x07)
    subc R3,R4          ; R3 = R3 - R4 - Carry (0x04 - 0x05 - 0 = 0xFFFF)

    ; Logical operations
    and R0,R2           ; R2 = R2 & R0 (0x07 & 0x01 = 0x01)
    or R1,R3            ; R3 = R3 | R1 (0xFFFF | 0x03 = 0xFFFF)
    xor R0,R4           ; R0 = R0 ^ R4 (0x01 ^ 0x05 = 0x04)

    ; Decimal Add operation
    movl #12,R5       ; Load 0x12 into R5
    movl #34,R6       ; Load 0x34 into R6
    dadd R5,R6          ; R6 = R6 + R5 in BCD (0x12 + 0x34 = 0x46)

    ; Move operations
    mov R5,R7           ; R7 = R5 (R7 should be 0x12)

    end Main
