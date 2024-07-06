#pragma once
/*
 Name: Semilore Kayode
 B00863866
 Course: ECED 3401
 Prepared for: Dr Hughes
 Purpose: This header file defines various macros (only some macros are defined here, not all) used in the project, including constants
 for memory size and return codes. These macros help improve code readability and
 maintainability by avoiding magic numbers.
 Date: 06/20/2024
*/

#ifndef MACROS_H
#define MACROS_H


#define OPCODE_GROUP_40 0x40 //Macro to represent group of instructions that begin with 0x40
#define OPCODE_MASK_40 0xF0 // Macro to represent masking group 0x40 instructions
#define OPCODE_MASK_MOV 0x0C // Macro to mask group 0x0C instructions
#define OPCODE_NO_INSTRUCTION 0x0000 // NO Instruction detected
#define REG_INDEX_PC 7
#define PC_INCREMENT 2



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
#define SETcc 23
#define CLRcc 24

// Defining a macro that represents a breakpoint check
#define BKPNT_CHECK (IMAR + 2)


#define NOP 0x4C09
// Defining check for clock at 0
#define ZERO_CLK (Clock == 0)
// Defining even clock tick for the pipeline
#define EVEN_CLK (Clock % 2 == 0)

/*
  Defining a constant for low bytes to always take the low bytes
  when dealing with byte implementation of instructions
*/
#define LOWBYTES_MASK 0x00FF
#define LOWNIB_MASK 0x000F
#define LSBit 0x01

#define SET 1
#define CLEAR 0

#define FIVE 5
#define FOUR 4
#define THREE 3
#define TWO 2
#define ONE 1
// Defining some commonly used values



#endif // !
