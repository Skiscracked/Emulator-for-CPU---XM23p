#pragma once
/*
 Name: Semilore Kayode
 B00863866
 Course: ECED 3401
 Prepared for: Dr Hughes
 Purpose: This header file contains the declaration of the Pipeline functions, which provide
 functionalities to simulate the execution of a pipeline cycle in a CPU. The fucntions
 include methods for initializing and executing pipeline stages.
 Date: 06/20/2024
*/
#ifndef PIPELINE_H
#define PIPELINE_H

typedef struct {
	int V;
	int N;
	int Z;
	int C;
    int SLP;
}ProgramStatusWord;

extern ProgramStatusWord SETCC, CLRCC;
//These are external variables for instructions which set/clear the PSW bits

extern unsigned carry[2][2][2];
// This is a 3d array that contains carry values that correspond to the result of signed and unsigned arithmetic
extern unsigned overflow[2][2][2];
// This is a 3d array that contains overflow values that correspond to the result of signed and unsigned arithmetic
extern unsigned offset_table[2][2][2];
// This is a 3d array that contains offset values that correspond to the decrement, prepost, and increment bits in a ld/st instruction


// Enum for condition prefixes concerning the CEX command
typedef enum {
    EQ = 0b0000, // Equal / equals zero
    NE = 0b0001, // Not equal
    CS = 0b0010, // Carry set / unsigned higher or same
    CC = 0b0011, // Carry clear / unsigned lower
    MI = 0b0100, // Minus / negative
    PL = 0b0101, // Plus / positive or zero
    VS = 0b0110, // Overflow
    VC = 0b0111, // No overflow
    HI = 0b1000, // Unsigned higher
    LS = 0b1001, // Unsigned lower or same
    GE = 0b1010, // Signed greater than or equal
    LT = 0b1011, // Signed less than
    GT = 0b1100, // Signed greater than
    LE = 0b1101, // Signed less than or equal
    TR = 0b1110, // Always true
    FL = 0b1111  // Always false
} ConditionCode;

extern ConditionCode Cond_prefix;//enum containing the condition prefix
extern bool CEX_state;// A bool variable to enable and disable CEX mode for the emulator
extern bool CEX_test;// A bool variable to tell us if the condition for the CEX is true or false

// These variables represent CPU registers for the XM23p:
// These are instruction memory registers
extern unsigned short IMAR;
extern unsigned short IMBR;
extern bool ICTRL;
extern unsigned short IR;
extern unsigned short LR;
extern unsigned short SP;
extern unsigned short Clock;

// These are data memory registers
extern unsigned short DMAR;
extern bool DCTRL;
extern unsigned short DMBR;
// This is the Effective Address for accessing data memory.
extern unsigned short EA;
extern int offset;
extern union w_b srcnum, dstnum, result;
// Declaring the variables used in the execute func to access src and dest as bytes/word

/* structures and unions below are used to execute the DADD instruction */
struct bcd_nibbles {
    unsigned short nib0 : 4; /* ":4" Denotes 4-bits */
    unsigned short nib1 : 4;
    unsigned short nib2 : 4;
    unsigned short nib3 : 4;
};

union bcd_word_nibble {
    unsigned short word;
    struct bcd_nibbles nibble;
};


void F0();// First stage of fetch
void F1();// Second stage of fetch
void D0();// Decode stage
void E0();// Execute stage
void E1();// Memory access stage
void bubble(); // Function to clear D0 and E0 getting rid of Data Hazards
void Run_pipeline_continuous();// Running the pipeline in continuous mode
void Run_pipeline_single();// Running the pipeline in increment mode by incrementing clock
void IMEM_Controller(unsigned int IMAR, unsigned int ICTRL, unsigned int * IMBR);
void execute_ADD();
void execute_ADDC();
void execute_SUB();
void execute_SUBC();
void execute_DADD();
void execute_CMP();
void execute_XOR();
void execute_AND();
void execute_OR();
void execute_BIT();
void execute_BIC();
void execute_BIS();
void execute_MOV();
void execute_SWAP();
void execute_SRA();
void execute_RRC();
void execute_SWPB();
void execute_SXT();
void execute_MOVL();
void execute_MOVLZ();
void execute_MOVLS();
void execute_MOVH();
void execute_SETCC();
void execute_CLRCC();
void Calculate_LD_Indexed();// Function to obtain the EA for LD (both direct and index addressing)
void Calculate_ST_Indexed();// Function to obtain the EA for ST (both direct and index addressing)
void Calculate_LDR();// Function to obtain the EA for LDR
void Calculate_STR();// Function to obtain the EA for STR
void execute_LD();// Function to execute LD
void execute_ST();// Function to execute ST
void execute_LDR();// Function to execute LDR
void execute_STR();// Function to execute STR
void execute_BL();
void execute_BEQ_BZ();
void execute_BNE_BNZ();
void execute_BC_BHS();
void execute_BNC_BLO();
void execute_BN();
void execute_BGE();
void execute_BLT();
void execute_BRA();
void execute_CEX();// This function turns on the CEX state after checking the condition

void CEX_checker(); // This function polls, checking if the instructions to executed are executed, and bubbles the ones to not be executed.
// This function is used in the pipeline, once CEX_state is SET in execute_CEX

extern char CEX_truecount;// These are variable of a byte each, used to store the count of true conditions upon decoding CEX
extern char CEX_falsecount;// These are variable of a byte each, used to store the count of false conditions upon decoding CEX

void update_psw(unsigned short src, unsigned short dest, unsigned short result, unsigned short wb); // This function updates VNZC
//void update_psw3(unsigned short dest, unsigned short wb); // This function updates NZC using 
void update_psw1(unsigned short result, unsigned short wb); // This function updates NZ using result
void update_psw2(unsigned short result); // This functions only updates Z using the result
unsigned short bcd_add(unsigned short nibble_A, unsigned short nibble_B);// Adding the nibbles of the 16 bit word in decimal add

void sigint_hdlr();
#endif // !
