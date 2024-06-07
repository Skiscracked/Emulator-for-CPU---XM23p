#pragma once
#ifndef DECODER_H
#define DECODER_H

#define REGFILE 8 /* Size of register file */
#define REGCON 2 /* RC bit: REG=0 or CONST=1 */
extern unsigned short reg_file[REGCON][REGFILE];

//Program Counter
extern unsigned int PC;

//IMEM starting address (from S1)
extern unsigned int IMEM_SA;

//Breakpoint variable to stop program
extern unsigned short breakpoint;

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
// Register functions
void display_and_process_debug_menu();
void display_registers();
void change_register_value(int reg, unsigned short value);
void change_memory_value(int type, unsigned short address, unsigned short value);
void set_breakpoint(unsigned short address);
#endif 
