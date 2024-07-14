; Sample Assembly Program to Test the XM23p Pipeline
; Author: Semilore Kayode
; Date: 29/06/2024

    V0 equ #0
    V2 equ #2
    V4 equ #4
    V6 equ #6
    V10 equ #10
    DATA
    org #0100
;
    Data1 word #ABCD
    Data2 word #FFFF
;
    CODE
    org #2000

Main
    movl Data1,R0     
    movh Data1,R0     ; R0 = 0100

    ld R0,R1          ; R1 <- mem[R0]. R1 = #ABCD
    ld.b R0,R2        ; R2 <- mem[R0]. R2 = #CD
    add #4,R0         ; R0 = 0104
    st R1,R0          ; mem[0104] = #ABCD
    add #2,R0         ; R0 = 0106
    st.b R2,R0        ; mem[0106] = #CD

    movl Data2,R0
    movh Data2,R0     ; R0 = 0102

    ldr R0,V0,R1      ; R1 = mem[R0 + 0] (#FFFF)
    str R1,R0,V6      ; mem[R0 + 6](0108) = R1 (#FFFF)
    ldr.b R0,V0,R3    ; R3 = mem[R0 + 0] = #FF
    str.b R3,R0,V10   ; mem[R0 + V10](0112) = #FF


    end Main
