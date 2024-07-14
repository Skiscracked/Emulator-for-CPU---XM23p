;
; Test Case 2: Handle Group 40 Instructions
; ECED 3403
; Date: 12/07/2024
;

    CODE
    org #1000

; Initialize registers
;
Main
    movlz  Array,R0  ; Load address of the array into R0. R0 = 0x0040
    ld     R0,R3      ; R3 <- mem[R0](0040). R3 = #5
    add    #2,R0      ; Move R0 to the next element. R0 = 0x0042
    ld     R0,R1      ; R1 <- mem[R0](0x0042). R1 = #1
    ld     +R0,R2     ; R0 = 0x0044. R2 <- mem[R0]. R2 = #2

    ; Group 40 Instructions
    add    R1,R2      ; ADD R1 to R2. R2 = #3
    addc   R1,R2      ; ADD with Carry R1 to R2. R2 = #4
    sub    R1,R2      ; SUB R1 from R2. R2 = #3
    subc   R1,R2      ; SUB with Carry R1 from R2
    dadd   R1,R2      ; Decimal ADD R1 to R2
    cmp    R1,R2      ; Compare R1 with R2
    xor    R1,R2      ; XOR R1 with R2
    and    R1,R2      ; AND R1 with R2
    or     R1,R2      ; OR R1 with R2
    bit    R1,R2      ; BIT test R1 with R2
    bic    R1,R2      ; BIC (Bit Clear) R1 from R2
    bis    R1,R2      ; BIS (Bit Set) R1 to R2

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

; Array
;
Array
    word  #5
    word  #1
    word  #2
    word  #3
    word  #4
    word  #5
;
    end Main
