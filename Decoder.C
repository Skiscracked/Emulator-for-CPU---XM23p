#include "Loader.h"
/*
 Name: Semilore Kayode
 B00863866
 Course: ECED 3401
 Prepared for: Dr Hughes
 Purpose: This module contains the implementation of the Decoder functions, which is responsible
 for decoding machine instructions. It extracts the opcode and operand from a given
 instruction and performs the corresponding operation.
 Date: 06/20/2024
*/

Instruction execute_input;

// Initializing the reg_file
unsigned short reg_file[REGCON][REGFILE] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 2, 4, 8, 16, 32, -1}
};
//Initializing/Re-declaring the extern variable
unsigned short breakpoint;

//Function to copy the instruction register - obtain and return the instruction
Instruction Copy_IR(unsigned int pc) {
    Instruction instr;
    instr.opcode = IR;
    instr.address = pc;
    return instr;
}

void decode_and_display()// This function is getting the starting address of the IMEM array, to be able to print instructions from that spot
{
    while (1)
    {
        Instruction command = Copy_IR(IMAR);
        if (((command.opcode >> 8) & 0xFF) == 0x4C)
            handle_group_4C(command);
        else if (((command.opcode >> 8) & 0xF0) == 0x40)// If it falls between the 0x40(0100 0000) range call this function
            handle_group_40(command);// Calling the function passing command, so the starting address of the IMEM can be obtained by handle_group_40
        else if (((command.opcode >> 3) & 0xFF0) == 0x9A4)
            handle_group_9A4(command);
        else if (((command.opcode >> 3) & 0xFFF0) == 0x9A0)
            handle_group_132(command);
        else if (((command.opcode >> 11) & 0x0C) == 0x0C)
            handle_group_MOV(command);
        else if (command.opcode == 0x0000)
        {
            printf("Program is now ending\n");
            break;
        }
        else
            printf("%04X: %04X\n", command.address, command.opcode);// Upon if the instruction is unidentified

        if (PC + 2 == breakpoint)//Algorithm to check for breakpoint
        {
            printf("Decoding has stopped\n");
            break;
        }

        PC += 2;// If PC+2 != breakpoint, increment PC by a byte pair
    }
    //Done:
    // I need to format this to print out the required output.
    // I also need to print out the other bits i.e. the constant, the destination etc.
    // A default case needs to be added to each switch statement
    //Need to do:
    // The second table (the move table) needs to be added
}

void handle_group_40(Instruction instr) //addy in this case is going to be the content of the PC.
// The PC is going to hold the next instructions
{
    unsigned int addy = instr.address;
    switch ((instr.opcode >> 8) & 0x000F)  // Masking to get the relevant bits (getting the lower 4 bits of the first 8 bit from the 16 bits) 
    {
    case 0x00: //ADD opcode
        printf("%04X: ADD", addy);// ADD Instruction
        display_content(instr);
        execute_input.UI = 1;
        break;
    case 0x01:
        printf("%04X: ADDC", addy);// ADDC Instruction
        display_content(instr);
        execute_input.UI = 2;
        break;
    case 0x02:
        printf("%04X: SUB", addy);// SUB Instruction
        display_content(instr);
        execute_input.UI = 3;
        break;
    case 0x03:
        printf("%04X: SUBC", addy);// SUBC Instruction
        display_content(instr);
        execute_input.UI = 4;
        break;
    case 0x04:
        printf("%04X: DADD", addy);// DADD Instruction
        display_content(instr);
        execute_input.UI = 5;
        break;
    case 0x05:
        printf("%04X: CMP", addy);// CMP Instruction
        display_content(instr);
        execute_input.UI = 6;
        break;
    case 0x06:
        printf("%04X: XOR", addy);// XOR Instruction
        display_content(instr);
        execute_input.UI = 7;
        break;
    case 0x07:
        printf("%04X: AND", addy);// AND Instruction
        display_content(instr);
        execute_input.UI = 8;
        break;
    case 0x08:
        printf("%04X: OR", addy);// OR Instruction
        display_content(instr);
        execute_input.UI = 9;
        break;
    case 0x09:
        printf("%04X: BIT", addy);// BIT Instruction
        display_content(instr);
        execute_input.UI = 10;
        break;
    case 0x0A:
        printf("%04X: BIC", addy);// BIC Instruction
        display_content(instr);
        execute_input.UI = 11;
        break;
    case 0x0B:
        printf("%04X: BIS", addy);// BIS Instruction
        display_content(instr);
        execute_input.UI = 12;
        break;
    default:
        printf("%04X: %04X\n", addy, instr.opcode);// Upon if the instruction is unidentified
        break;
    }

}

void handle_group_4C(Instruction instr)
{
    unsigned int addy;
    addy = instr.address;
    switch ((instr.opcode >> 7) & 0x03)//Shifting and masking 
    {
    case 0x00:
        printf("%04X: MOV", addy); //MOV Instruction
        //display_content(instr);
        instr.w_b = ((instr.opcode >> 6) & 0x01);
        instr.s_c = ((instr.opcode >> 3) & 0x07);
        instr.dest = ((instr.opcode) & 0x07);

        if ((instr.s_c <= 7) && (instr.dest <= 7))
            printf(" WB: %d SRC: R%d DST: R%d\n", instr.w_b, instr.s_c, instr.dest);

        execute_input.UI = 13;
        break;
    case 0x01:
        printf("%04X: SWAP", addy);//SWAP Instruction
        instr.s_c = ((instr.opcode >> 3) & 0x07);
        instr.dest = ((instr.opcode) & 0x07);
        printf(" S: %d DST: R%d\n", instr.s_c, instr.dest);
        execute_input.UI = 14;
        break;
    default:
        printf("%04X: %04X\n", addy, instr.opcode);// Upon if the instruction is unidentified
        break;
    }
}

void handle_group_132(Instruction instr)
{
    unsigned int addy;
    addy = instr.address;
    switch ((instr.opcode >> 3) & 0x01)
    {
    case 0x00:
        printf("%04X: SRA", addy); //SRA Instruction
        display_content_4_SRA_and_RRC(instr);
        execute_input.UI = 15;
        break;
    case 0x01:
        printf("%04X: RRC", addy);//RRC Instruction
        display_content_4_SRA_and_RRC(instr);
        execute_input.UI = 16;
        break;
    default:
        printf("%04X: %04X\n", addy, instr.opcode);// Upon if the instruction is unidentified
        break;
    }
}

void handle_group_9A4(Instruction instr)
{
    unsigned int addy;
    addy = instr.address;
    switch ((instr.opcode >> 3) & 0x000F)
    {
    case 0x03:
        printf("%04X: SWPB", addy);//SWPB Instruction
        instr.dest = ((instr.opcode) & 0x07);

        if (instr.dest <= 7)
            printf(" DST: R%d\n", instr.dest);
        execute_input.UI = 17;
        break;

    case 0x04:
        printf("%04X: SXT", addy);//SXT Instruction
        instr.dest = ((instr.opcode) & 0x07);

        if (instr.dest <= 7)
            printf(" DST: R%d\n", instr.dest);
        execute_input.UI = 18;
        break;
    default:
        printf("%04X: %04X\n", addy, instr.opcode);// Upon if the instruction is unidentified
        break;
    }
}

void handle_group_MOV(Instruction instr)
{
    unsigned int addy;
    addy = instr.address;
    Instruction temp = instr;
    switch ((instr.opcode >> 11) & 0x0F)
    {
    case 0x0C:
        extract_data_and_dest(&temp);
        printf("%04X: MOVL DST.Low: %x DST: R%x\n", addy, temp.data, temp.dest);
        execute_input.UI = 19;
        break;
    case 0x0D:
        extract_data_and_dest(&temp);
        printf("%04X: MOVLZ DST.Low: %x DST: R%x\n", addy, temp.data, temp.dest);
        execute_input.UI = 20;
        break;
    case 0x0E:
        extract_data_and_dest(&temp);
        printf("%04X: MOVLS DST.Low: %x DST: R%x\n", addy, temp.data, temp.dest);
        execute_input.UI = 21;
        break;
    case 0x0F:
        extract_data_and_dest(&temp);
        printf("%04X: MOVH DST.High: %x DST: R%x\n", addy, temp.data, temp.dest);
        execute_input.UI = 22;
        break;
    default:
        printf("%04X: %04X\n", addy, temp.opcode);// Upon if the instruction is unidentified
        break;
    }
}

void extract_data_and_dest(Instruction* instr_handler)
{
    instr_handler->dest = (instr_handler->opcode & 0x07);
    instr_handler->data = ((instr_handler->opcode >> 3) & 0xFF);

    execute_input = *instr_handler;// After decoding instruction, output it to the input for execute
}

void display_content(Instruction content)
{
    content.r_c = ((content.opcode >> 7) & 0x01);
    content.w_b = ((content.opcode >> 6) & 0x01);
    content.s_c = ((content.opcode >> 3) & 0x07);
    content.dest = ((content.opcode) & 0x07);


    execute_input = content;// After decoding instruction output, it to the input for execute

    if (content.r_c == 0)
        printf(" RC: %d WB: %d SRC: R%d DST: R%d\n", content.r_c, content.w_b, content.s_c, content.dest);

    else if (content.r_c == 1)
        printf(" RC: %d WB: %d CON: %d DST: R%d\n", content.r_c, content.w_b, content.s_c, content.dest);
}

void display_content_4_SRA_and_RRC(Instruction content)
{
    content.w_b = ((content.opcode >> 6) & 0x01);
    content.dest = ((content.opcode) & 0x07);

    if (content.dest <= 7)
        printf(" WB: %d DST: R%d\n", content.w_b, content.dest);

    execute_input = content;// After decoding instruction, output it to the input for execute
}

void display_and_process_debug_menu()
{
    int choice;
    int reg, type;
    unsigned short address, value;
    
    while (1)
    {
        printf("Debugger Commands:\n");
        printf("1. Display Register Values\n");
        printf("2. Change Register Value\n");
        printf("3. Change Memory Value\n");
        printf("4. Set Breakpoint\n");
        printf("5. Previous Menu\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            display_registers();
            break;
        case 2:
            printf("Enter register number (0-7): ");
            scanf("%d", &reg);
            printf("Enter new value (hex): ");
            scanf("%hx", &value);
            change_register_value(reg, value);
            break;
        case 3:
            printf("Enter memory type (0 for instruction, 1 for data): ");
            scanf("%d", &type);
            printf("Enter address (hex): ");
            scanf("%hx", &address);
            printf("Enter new value (hex): ");
            scanf("%hx", &value);
            change_memory_value(type, address, value);
            break;
        case 4:
            printf("Enter breakpoint address (hex): ");
            scanf("%hx", &address);
            set_breakpoint(address);
            break;
        case 5:
            return;

        default:
            printf("Invalid choice.\n");
        }
    }
}

void display_registers() 
{
    for (int i = 0; i < REGFILE; i++) 
        printf("R%d: 0x%04X\n", i, reg_file[0][i]);
}

void change_register_value(int reg, unsigned short value)
{
    if (reg >= 0 && reg < REGFILE) {
        if (value <= 0xFFFF)
        {
            reg_file[0][reg] = value;
            if (reg == 7)
                PC = value;// Register 7 holds the program counter
        }
        else
            printf("Value is out of bounds. It needs to be less than FFFF\n");
    }
    else {
        printf("Invalid register number.\n");
    }
}

void change_memory_value(int type, unsigned short address, unsigned short value)
{
    if (value < 65536)
    {
        if (type == 0) {
            // Instruction memory
            IMEM.bytmem[address] = (value & 0x00FF);
            IMEM.bytmem[address + 1] = (value >> 8);
        }
        else if (type == 1) {
            // Data memory
            DMEM.bytmem[address] = (value & 0x00FF);
            DMEM.bytmem[address + 1] = (value >> 8);
        }
        else {
            printf("Invalid memory type.\n");
        }
    }

    else
        printf("Segmentation fault. Value is outside of boundary");
}

void set_breakpoint(unsigned short address) 
{
    breakpoint = address;
}