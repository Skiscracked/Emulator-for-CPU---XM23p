#include "Loader.h"

//Function to fetch - obtain and return the instruction
Instruction fetch(unsigned int pc) {
    Instruction instr;
    instr.opcode = (IMEM[pc + 1] << 8) | IMEM[pc];
    instr.address = pc;
    return instr;
}

void decode_and_display(unsigned int SA)// This function is getting the starting address of the IMEM array, to be able to print instructions from that spot
{
    PC = SA; // Giving the PC the starting address of the IMEM

    while (1)
    {
        Instruction command = fetch(PC);
        if (((command.opcode >> 8) & 0xF0) == 0x40)// If it falls between the 0x40(0100 0000) range call this function
            handle_group_40(command);// Calling the function passing command, so the starting address of the IMEM can be obtained by handle_group_40
        else if (((command.opcode >> 8) & 0xFF) == 0x4C)
            handle_group_4C(command);
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

        PC += 2;// Increment PC by a byte pair
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
        break;
    case 0x01:
        printf("%04X: ADDC", addy);// ADDC Instruction
        display_content(instr);
        break;
    case 0x02:
        printf("%04X: SUB", addy);// SUB Instruction
        display_content(instr);
        break;
    case 0x03:
        printf("%04X: SUBC", addy);// SUBC Instruction
        display_content(instr);
        break;
    case 0x04:
        printf("%04X: DADD", addy);// DADD Instruction
        display_content(instr);
        break;
    case 0x05:
        printf("%04X: CMP", addy);// CMP Instruction
        display_content(instr);
        break;
    case 0x06:
        printf("%04X: XOR", addy);// XOR Instruction
        display_content(instr);
        break;
    case 0x07:
        printf("%04X: AND", addy);// AND Instruction
        display_content(instr);
        break;
    case 0x08:
        printf("%04X: OR", addy);// OR Instruction
        display_content(instr);
        break;
    case 0x09:
        printf("%04X: BIT", addy);// BIT Instruction
        display_content(instr);
        break;
    case 0x0A:
        printf("%04X: BIC", addy);// BIC Instruction
        display_content(instr);
        break;
    case 0x0B:
        printf("%04X: BIS", addy);// BIS Instruction
        display_content(instr);
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
        break;
    case 0x01:
        printf("%04X: SWAP", addy);//SWAP Instruction
        instr.s_c = ((instr.opcode >> 3) & 0x07);
        instr.dest = ((instr.opcode) & 0x07);
        printf(" S: %d DST: R%d\n", instr.s_c, instr.dest);
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
        break;
    case 0x01:
        printf("%04X: RRC", addy);//RRC Instruction
        display_content_4_SRA_and_RRC(instr);
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
        break;

    case 0x04:
        printf("%04X: SXT", addy);//SXT Instruction
        instr.dest = ((instr.opcode) & 0x07);

        if (instr.dest <= 7)
            printf(" DST: R%d\n", instr.dest);
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
        break;
    case 0x0D:
        extract_data_and_dest(&temp);
        printf("%04X: MOVLZ DST.Low: %x DST: R%x\n", addy, temp.data, temp.dest);
        break;
    case 0x0E:
        extract_data_and_dest(&temp);
        printf("%04X: MOVLS DST.Low: %x DST: R%x\n", addy, temp.data, temp.dest);
        break;
    case 0x0F:
        extract_data_and_dest(&temp);
        printf("%04X: MOVL DST.High: %x DST: R%x\n", addy, temp.data, temp.dest);
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
}

void display_content(Instruction content)
{
    content.r_c = ((content.opcode >> 7) & 0x01);
    content.w_b = ((content.opcode >> 6) & 0x01);
    content.s_c = ((content.opcode >> 3) & 0x07);
    content.dest = ((content.opcode) & 0x07);

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

}