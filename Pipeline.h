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
extern unsigned overflow[2][2][2];
extern unsigned offset_table[2][2][2];

// These variables represent CPU registers for the XM23p

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


void update_psw(unsigned short src, unsigned short dest, unsigned short result, unsigned short wb); // This function updates VNZC
//void update_psw3(unsigned short dest, unsigned short wb); // This function updates NZC using 
void update_psw1(unsigned short result, unsigned short wb); // This function updates NZ using result
void update_psw2(unsigned short result); // This functions only updates Z using the result
unsigned short bcd_add(unsigned short nibble_A, unsigned short nibble_B);// Adding the nibbles of the 16 bit word in decimal add
#endif // !
