#pragma once
#ifndef DECODER_H
#define DECODER_H

//Program Counter
extern unsigned int PC;

//IMEM starting address (from S1)
extern unsigned int IMEM_SA;

//Declaring an instruction
typedef struct {
    unsigned int opcode;//holds the binary equivalent of the bytes from the array
    unsigned int address;
    unsigned char r_c;
    unsigned char w_b;
    unsigned char s_c;
    unsigned char dest;
    unsigned int data;
} Instruction;

void decode_and_display(unsigned int SA);
void display_content(Instruction content);
void display_content_4_SRA_and_RRC(Instruction content);
void handle_group_40(Instruction instr);
void handle_group_4C(Instruction instr);
void handle_group_132(Instruction instr);
void handle_group_9A4(Instruction instr);
void handle_group_MOV(Instruction instr);
void extract_data_and_dest(Instruction* instr_handler);

#endif 
