#pragma once
/*
 Name: Semilore Kayode
 B00863866
 Course: ECED 3401
 Prepared for: Dr Hughes
 Purpose: This header file defines various macros used in the project, including constants
 for memory size and return codes. These macros help improve code readability and
 maintainability by avoiding magic numbers.
 Date: 06/20/2024
*/

#ifndef MACROS_H
#define MACROS_H

// Defining UI(Unique Identifiers) for the different execute functions
#define ADD 1
#define ADDC 2
#define SUB 3
#define SUBC 4
#define DADD 5
#define CMP 6
#define XOR 7
#define AND 8
#define OR 9
#define BIT 10
#define BIC 11
#define BIS 12
#define MOV 13
#define SWAP 14
#define SRA 15
#define RRC 16
#define SWPB 17
#define SXT 18
#define MOVL 19
#define MOVLZ 20
#define MOVLS 21
#define MOVH 22

#define BKPNT_CHECK (PC + 2)


#define NOP 0x4C09
// Defining check for clock at 0
#define ZERO_CLK (Clock == 0)
// Defining even clock tick for the pipeline
#define EVEN_CLK (Clock % 2 == 0)


#endif // !
