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
ProgramStatusWord SETCC, CLRCC;
ConditionCode Cond_prefix;//enum containing the condition prefix
bool CEX_state;// A bool variable to enable and disable CEX mode for the emulator
char CEX_truecount;
char CEX_falsecount;

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
        if (((command.opcode >> 8) & OPCODE_MASK_40) == OPCODE_GROUP_40)// If it falls between the 0x40(0100 0000) range call this function
            handle_group_40(command);// Calling the function passing command, so the starting address of the IMEM can be obtained by handle_group_40
        else if (((command.opcode >> 11) & OPCODE_MASK_MOV) == OPCODE_MASK_MOV)
            handle_group_MOV(command);
        else if (command.opcode == OPCODE_NO_INSTRUCTION)
        {
            printf("Program is now ending\n");
            execute_input.UI = 0;
            return;
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
    switch ((instr.opcode >> 8) & LOWNIB_MASK)  // Masking to get the relevant bits (getting the lower 4 bits of the first 8 bit from the 16 bits) 
    {
    case 0x00: //ADD opcode
        //printf("%04X: ADD", addy);// ADD Instruction
        display_content(instr);
        execute_input.UI = ADD;
        break;
    case 0x01:
        //printf("%04X: ADDC", addy);// ADDC Instruction
        display_content(instr);
        execute_input.UI = ADDC;
        break;
    case 0x02:
        //printf("%04X: SUB", addy);// SUB Instruction
        display_content(instr);
        execute_input.UI = SUB;
        break;
    case 0x03:
        //printf("%04X: SUBC", addy);// SUBC Instruction
        display_content(instr);
        execute_input.UI = SUBC;
        break;
    case 0x04:
        //printf("%04X: DADD", addy);// DADD Instruction
        display_content(instr);
        execute_input.UI = DADD;
        break;
    case 0x05:
        //printf("%04X: CMP", addy);// CMP Instruction
        display_content(instr);
        execute_input.UI = CMP;
        break;
    case 0x06:
        //printf("%04X: XOR", addy);// XOR Instruction
        display_content(instr);
        execute_input.UI = XOR;
        break;
    case 0x07:
        //printf("%04X: AND", addy);// AND Instruction
        display_content(instr);
        execute_input.UI = AND;
        break;
    case 0x08:
        //printf("%04X: OR", addy);// OR Instruction
        display_content(instr);
        execute_input.UI = OR;
        break;
    case 0x09:
        //printf("%04X: BIT", addy);// BIT Instruction
        display_content(instr);
        execute_input.UI = BIT;
        break;
    case 0x0A:
        //printf("%04X: BIC", addy);// BIC Instruction
        display_content(instr);
        execute_input.UI = BIC;
        break;
    case 0x0B:
        //printf("%04X: BIS", addy);// BIS Instruction
        display_content(instr);
        execute_input.UI = BIS;
        break;
    case 0x0C:
        handle_group_4C(instr);
        break;
    case 0x0D:
        if (((instr.opcode >> 7) & 0x01) == 0)
            handle_group_132(instr);
        else
            handle_SETCC_and_CLRCC(instr);
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
        //printf("%04X: MOV", addy); //MOV Instruction
        //display_content(instr);
        instr.w_b = ((instr.opcode >> 6) & 0x01);
        instr.s_c = ((instr.opcode >> 3) & 0x07);
        instr.dest = ((instr.opcode) & 0x07);
        instr.r_c = 0x00;
        execute_input = instr;
        execute_input.UI = MOV;
        break;
    case 0x01:
        //printf("%04X: SWAP", addy);//SWAP Instruction
        instr.s_c = ((instr.opcode >> 3) & 0x07);
        instr.dest = ((instr.opcode) & 0x07);
        //printf(" SRC: R%d DST: R%d\n", instr.s_c, instr.dest);
        execute_input = instr;
        execute_input.UI = SWAP;
        break;
    }
}

void handle_group_132(Instruction instr)
{
    unsigned int addy;
    addy = instr.address;
    switch ((instr.opcode >> 3) & 0x07)
    {
    case 0x00:
        //printf("%04X: SRA", addy); //SRA Instruction
        display_content_4_SRA_and_RRC(instr);
        execute_input.UI = SRA;
        break;
    case 0x01:
        //printf("%04X: RRC", addy);//RRC Instruction
        display_content_4_SRA_and_RRC(instr);
        execute_input.UI = RRC;
        break;
    case 0x03:
        //printf("%04X: SWPB", addy);//SWPB Instruction
        instr.dest = ((instr.opcode) & 0x07);

        /*if (instr.dest <= 7)
            printf(" DST: R%d\n", instr.dest);*/
        execute_input = instr;// After decoding instruction, output it to the input for execute
        execute_input.UI = SWPB;
        break;

    case 0x04:
        //printf("%04X: SXT", addy);//SXT Instruction
        instr.dest = ((instr.opcode) & 0x07);

        if (instr.dest <= 7)
            //printf(" DST: R%d\n", instr.dest);
        execute_input = instr;// After decoding instruction, output it to the input for execute
        execute_input.UI = SXT;
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
        //printf("%04X: MOVL DST.Low: %x DST: R%x\n", addy, temp.data, temp.dest);
        execute_input.UI = MOVL;
        break;
    case 0x0D:
        extract_data_and_dest(&temp);
        //printf("%04X: MOVLZ DST.Low: %x DST: R%x\n", addy, temp.data, temp.dest);
        execute_input.UI = MOVLZ;
        break;
    case 0x0E:
        extract_data_and_dest(&temp);
        //printf("%04X: MOVLS DST.Low: %x DST: R%x\n", addy, temp.data, temp.dest);
        execute_input.UI = MOVLS;
        break;
    case 0x0F:
        extract_data_and_dest(&temp);
        //printf("%04X: MOVH DST.High: %x DST: R%x\n", addy, temp.data, temp.dest);
        execute_input.UI = MOVH;
        break;
    }
}

void extract_data_and_dest(Instruction* instr_handler)
{
    instr_handler->dest = (instr_handler->opcode & 0x07);
    instr_handler->data = ((instr_handler->opcode >> 3) & 0xFF);

    execute_input = *instr_handler;// After decoding instruction, output it to the input for execute
}

void handle_SETCC_and_CLRCC(Instruction instr)
{
    Instruction temp = instr;
    execute_input = temp;
    switch ((temp.opcode >> FIVE) & THREE)
    {
    case 0x01:
        SETCC.V = (temp.opcode >> FOUR) & LSBit;
        SETCC.SLP = (temp.opcode >> THREE) & LSBit;
        SETCC.N = (temp.opcode >> TWO) & LSBit;
        SETCC.Z = (temp.opcode >> ONE) & LSBit;
        SETCC.C = (temp.opcode) & LSBit;
        execute_input.UI = SETcc;
        break;
    case 0x02:
        CLRCC.V = (temp.opcode >> FOUR) & LSBit;
        CLRCC.SLP = (temp.opcode >> THREE) & LSBit;
        CLRCC.N = (temp.opcode >> TWO) & LSBit;
        CLRCC.Z = (temp.opcode >> ONE) & LSBit;
        CLRCC.C = (temp.opcode) & LSBit;
        execute_input.UI = CLRcc;
        break;
    }
    
}

void handle_group_LD_and_ST(Instruction instr)
{
    Instruction handler = instr;
    switch ((handler.opcode >> TEN) & LSBit)
    {
    case 0:
        // This handles decoding data for instruction - LD
        handler.dest = (handler.opcode & SEVEN);
        handler.s_c = ((handler.opcode >> THREE) & SEVEN);
        handler.w_b = ((handler.opcode >> SIX) & LSBit);
        handler.INC = ((handler.opcode >> SEVEN) & LSBit);
        handler.DEC = ((handler.opcode >> EIGHT) & LSBit);
        handler.PRPO = ((handler.opcode >> NINE) & LSBit);
        handler.UI = LD;
        break;
    case 1:
        // This handles decoding data for instruction - ST
        handler.dest = (handler.opcode & SEVEN);
        handler.s_c = ((handler.opcode >> THREE) & SEVEN);
        handler.w_b = ((handler.opcode >> SIX) & LSBit);
        handler.INC = ((handler.opcode >> SEVEN) & LSBit);
        handler.DEC = ((handler.opcode >> EIGHT) & LSBit);
        handler.PRPO = ((handler.opcode >> NINE) & LSBit);
        handler.UI = ST;
        break;

    }   
    execute_input = handler;
}

void handle_group_LDR_and_STR(Instruction instr)
{
    Instruction handler = instr;
    switch (handler.opcode >> 14)
    {
    case TWO:
        // This handles decoding data for instruction - LDR
        handler.dest = (handler.opcode & SEVEN);
        handler.s_c = ((handler.opcode >> THREE) & SEVEN);
        handler.w_b = ((handler.opcode >> SIX) & LSBit);
        handler.OFF = ((handler.opcode >> SEVEN) & OFFSET_MASK);
        sign_extend_7bit(&handler.OFF);
        handler.UI = LDR;
        break;
    case THREE:
        // This handles decoding data for instruction - STR
        handler.dest = (handler.opcode & SEVEN);
        handler.s_c = ((handler.opcode >> THREE) & SEVEN);
        handler.w_b = ((handler.opcode >> SIX) & LSBit);
        handler.OFF = ((handler.opcode >> SEVEN) & OFFSET_MASK);
        sign_extend_7bit(&handler.OFF);
        handler.UI = STR;
        break;
    }
    execute_input = handler;
}

void handle_BL(Instruction instr)
{ // Handles instruction BL - Branch with link
    Instruction handler = instr;
    handler.OFF_13bit = (handler.opcode & BL_OFFSET_MASK);
    sign_extend_13bit(&handler.OFF_13bit);
    handler.UI = BL;
    execute_input = handler;
}

void handle_BRANCH_group(Instruction instr)
{ // Handles instructions from BEQ - BRA
    Instruction handler = instr;
    switch (handler.opcode >> TEN)
    {
    case EIGHT: // BEQ/BZ
        handler.UI = BEQ_BZ;
        break;
    case NINE:// BNE/BNZ
        handler.UI = BNE_BNZ;
        break;
    case TEN: // BC/BHS
        handler.UI = BC_BHS;
        break;
    case ELEVEN: // BNC/BLO
        handler.UI = BNC_BLO;
        break;
    case TWELVE: // BN
        handler.UI = BN;
        break;
    case THIRTEEN: // BGE
        handler.UI = BGE;
        break;
    case FOURTEEN: // BLT
        handler.UI = BLT;
        break;
    case FIFTEEN: // BRA
        handler.UI = BRA;
        break;
    }
    handler.OFF_10bit = (handler.opcode & BRANCH_MASK);
    sign_extend_10bit(&handler.OFF_10bit);
    execute_input = handler;
}

void handle_CEX(Instruction instr)
{   
    // This function handles the decoding(obtaining of the various bits/data) of the CEX instruction
   
    Instruction handler = instr;
    handler.Condition = (handler.opcode >> SIX) & FIFTEENhex; // Obtaining the condition from the opcode
    int x = handler.Condition; 
    // Giving a variable of type int the condition prefix, so I can give the variable of type enum the decoded prefix
    Cond_prefix = x;
    handler.True_count = (handler.opcode >> THREE) & SEVENhex;// Obtaining the number of instruction to execute if true from the opcode
    handler.False_count = (handler.opcode & SEVENhex);// Obtaining the number of instruction to execute if false from the opcode
    CEX_truecount = handler.True_count;
    CEX_falsecount = handler.False_count;
    handler.UI = CEX;// Assigning a value to the execute_input unique identifier for the functions
    execute_input = handler;
}

void display_content(Instruction content)
{
    content.r_c = ((content.opcode >> 7) & 0x01);
    content.w_b = ((content.opcode >> 6) & 0x01);
    content.s_c = ((content.opcode >> 3) & 0x07);
    content.dest = ((content.opcode) & 0x07);


    execute_input = content;// After decoding instruction output, it to the input for execute
}

void display_content_4_SRA_and_RRC(Instruction content)
{
    content.w_b = ((content.opcode >> 6) & 0x01);
    content.dest = ((content.opcode) & 0x07);

    /*if (content.dest <= 7)
        printf(" WB: %d DST: R%d\n", content.w_b, content.dest);*/

    execute_input = content;// After decoding instruction, output it to the input for execute
}

void sign_extend_7bit(signed char *off)
{
    if (SIGN(*off)) //MSB = 1
    // If it's a negative number
    {
        *off = (*off) | 0x80;
    }
    else
    {
        *off = *off & 0x003F;
    }
}

void sign_extend_13bit(signed short* OFF_13bit)
{
    if (SIGN_13(*OFF_13bit)) // If MSB == 1
    { // If it's a negative number
        *OFF_13bit = *OFF_13bit | 0xF000;
    }
    else // If MSB == 0
    {
        *OFF_13bit = *OFF_13bit & 0x1FFF;
    }
    *OFF_13bit = *OFF_13bit << 1;
}

void sign_extend_10bit(signed short *OFF_10bit)
{
    if (SIGN_10(*OFF_10bit)) // If MSB == 1
    { // If it's a negative number
        *OFF_10bit = *OFF_10bit | 0xFC00;
    }
    else // If MSB == 0
    {
        *OFF_10bit = *OFF_10bit & 0x03FF;
    }
    *OFF_10bit = *OFF_10bit << 1;
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
        if (value <= (MEM_SIZE -1))
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
    if (value < MEM_SIZE)
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