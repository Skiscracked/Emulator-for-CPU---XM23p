#pragma once
/*
 Name: Semilore Kayode
 B00863866
 Course: ECED 3401
 Prepared for: Dr Hughes
 Purpose: This header file contains the declaration of the Decoder functions, which provide
 functionalities to decode machine instructions. The functions include methods for
 decoding and processing instructions.
 Date: 06/20/2024
*/
#ifndef DECODER_H
#define DECODER_H

#define REGFILE 8 /* Size of register file */
#define REGCON 2 /* RC bit: REG=0 or CONST=1 */
extern unsigned short reg_file[REGCON][REGFILE];

//Program Counter
extern unsigned short PC;

//IMEM starting address (from S1)
extern unsigned short IMEM_SA;

//Breakpoint variable to stop program
extern unsigned short breakpoint;

//Declaring an instruction
typedef struct {
    unsigned short opcode;//holds the binary equivalent of the bytes from the array
    unsigned short address;
    unsigned char r_c;
    unsigned char PRPO;
    unsigned char DEC;
    unsigned char INC;
    unsigned char w_b;
    unsigned char s_c;
    unsigned char dest;
    unsigned short data; // Data for the MOVH, MOVL, MOVLZ and MOVLS functions
    signed char OFF;
    signed short OFF_13bit;
    signed short OFF_10bit;
    unsigned short UI; // Unique Identifier for Instructions
} Instruction;

extern Instruction command;
// This struct is the input for decode

extern Instruction execute_input;
// This struct is populate by the Decode function and is an input for execute


void decode_and_display();
void display_content(Instruction content);
void display_content_4_SRA_and_RRC(Instruction content);
void handle_group_40(Instruction instr);
void handle_group_4C(Instruction instr);
void handle_group_132(Instruction instr);
void handle_group_MOV(Instruction instr);
void extract_data_and_dest(Instruction* instr_handler);
void handle_SETCC_and_CLRCC(Instruction instr);
void handle_group_LD_and_ST(Instruction instr);
void handle_group_LDR_and_STR(Instruction instr);
void handle_BL(Instruction instr);
void handle_BRANCH_group(Instruction instr);

// Register functions
Instruction Copy_IR(unsigned int pc);
void sign_extend_7bit(signed char *off);
void sign_extend_13bit(signed short *OFF_13bit);
void sign_extend_10bit(signed short *OFF_10bit);
void display_and_process_debug_menu();
void display_registers();
void change_register_value(int reg, unsigned short value);
void change_memory_value(int type, unsigned short address, unsigned short value);
void set_breakpoint(unsigned short address);
#endif 
