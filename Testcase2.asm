;
; Test Case 2: Handle Group 40 Instructions
; ECED 3403
; Date: Today
;

    CODE
    org #1000

; Initialize registers
;
Main
    movlz  Array,R0  ; Load address of the array into R0
    ld     R0,R3      ; Load stopper into R3
    add    #2,R0      ; Move R0 to the next element
    mov    R1,R2      ; Setting R2 as sum register and making it 0

    ; Group 40 Instructions
    add    R1,R2      ; ADD R1 to R2
    addc   R1,R2      ; ADD with Carry R1 to R2
    sub    R1,R2      ; SUB R1 from R2
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
    word  #0
    word  #1
    word  #1
    word  #1
    word  #1
    word  #1

; Storage for the sum result
;
    end Main
