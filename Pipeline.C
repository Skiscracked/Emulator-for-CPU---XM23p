#include "Loader.h"
/*
 Name: Semilore Kayode
 B00863866
 Course: ECED 3401
 Prepared for: Dr Hughes
 Purpose: This module contains the implementation of the Pipeline functions, which simulates
 the execution of a pipeline cycle in a CPU. The functions provides methods to
 initialize and execute pipeline stages.
 Date: 06/20/2024
*/
unsigned short IMAR, IMBR, IR, Clock;
unsigned short DMAR, DMBR, EA;
bool ICTRL, DCTRL;
ProgramStatusWord PSW, SETCC, CLRCC;
int offset;
volatile sig_atomic_t ctrl_c_fnd; /* T|F - indicates whether ^C detected */
bool CEX_test;// A bool variable to tell us if the condition for the CEX is true or false
char CEX_truecount;// Declaring the extern variables in my .C, to use them locally but store them globally
char CEX_falsecount;// Declaring the extern variables in my .C, to use them locally but store them globally

unsigned carry[2][2][2] = { 0, 0, 1, 0, 1, 0, 1, 1 };
unsigned overflow[2][2][2] = { 0, 1, 0, 0, 0, 0, 1, 0 };
unsigned offset_table[2][2][2] = { 0, 0, 2, 1, -2, -1, 0, 0 };

union w_b srcnum, dstnum, result;

void F0()
{
	IMAR = PC;
	ICTRL = 0; // Signal to RD word


	PC += PC_INCREMENT;
    reg_file[0][REG_INDEX_PC] = PC; // Giving register 7 the value of PC. To update R7 and for debugging purposes
}

void F1()
{
	IMEM_Controller(IMAR, ICTRL, &IMBR);
	IR = IMBR;
}

void IMEM_Controller(unsigned int IMAR, unsigned int ICTRL, unsigned int* IMBR)
{
	if (ICTRL == 0)
	*IMBR = IMEM.wrdmem[IMAR>>1];
}

void D0()
{ 
     Instruction command = Copy_IR(IMAR);
     if (((command.opcode >> EIGHT) & OPCODE_MASK_40) == OPCODE_GROUP_40)// If it falls between the 0x40(0100 0000) range call this function
         handle_group_40(command);// Calling the function passing command, so the starting address of the IMEM can be obtained by handle_group_40
     else if (((command.opcode >> ELEVEN) & OPCODE_MASK_MOV) == OPCODE_MASK_MOV)
         handle_group_MOV(command);
     else if (((command.opcode >> ELEVEN) & LOWNIB_MASK) == OPCODE_MASK_LD_ST)
         handle_group_LD_and_ST(command);
     else if (((command.opcode >> FOURTEEN) & TWO) == TWO)
         handle_group_LDR_and_STR(command);
     else if ((command.opcode >> THIRTEEN) == 0)
         handle_BL(command);
     else if ((command.opcode >> THIRTEEN) == ONE)
         handle_BRANCH_group(command);
     else if ((command.opcode >> ELEVEN) == TEN)
         handle_CEX(command);
     else if (command.opcode == OPCODE_NO_INSTRUCTION)
     {
         printf("Program is now ending\n");
         execute_input.UI = 0;
         return;
     }

}

void E0()
{
    switch (execute_input.UI)
    {
    case ADD:
        execute_ADD();
        break;
    case ADDC:
        execute_ADDC();
        break;
    case SUB:
        execute_SUB();
        break;
    case SUBC:
        execute_SUBC();
        break;
    case DADD:
        execute_DADD();
        break;
    case CMP:
        execute_CMP();
        break;
    case XOR:
        execute_XOR();
        break;
    case AND:
        execute_AND();
        break;
    case OR:
        execute_OR();
        break;
    case BIT:
        execute_BIT();
        break;
    case BIC:
        execute_BIC();
        break;
    case BIS:
        execute_BIS();
        break;
    case MOV:
        execute_MOV();
        break;
    case SWAP:
        execute_SWAP();
        break;
    case SRA:
        execute_SRA();
        break;
    case RRC:
        execute_RRC();
        break;
    case SWPB:
        execute_SWPB();
        break;
    case SXT:
        execute_SXT();
        break;
    case MOVL:
        execute_MOVL();
        break;
    case MOVLZ:
        execute_MOVLZ();
        break;
    case MOVLS:
        execute_MOVLS();
        break;
    case MOVH:
        execute_MOVH();
        break;
    case SETcc:
        execute_SETCC();
        break;
    case CLRcc:
        execute_CLRCC();
        break;
    case LD:
        Calculate_LD_Indexed();
        break;
    case ST:
        Calculate_ST_Indexed();
        break;
    case LDR:
        Calculate_LDR();
        break;
    case STR:
        Calculate_STR();
        break;
    case BL:
        execute_BL();
        break;
    case BEQ_BZ:
        execute_BEQ_BZ();
        break;
    case BNE_BNZ:
        execute_BNE_BNZ();
        break;
    case BC_BHS:
        execute_BC_BHS();
        break;
    case BNC_BLO:
        execute_BNC_BLO();
        break;
    case BN:
        execute_BN();
        break;
    case BGE:
        execute_BGE();
        break;
    case BLT:
        execute_BLT();
        break;
    case BRA:
        execute_BRA();
        break;
    case CEX:
        execute_CEX();
        break;
    }
}

void E1()
{
    printf("%d\t\t\t\t\t\t\t\tE1: %04X\n", Clock, execute_input.opcode);
    switch (execute_input.UI)
    {
    case LD:
        execute_LD();
        break;
    case ST:
        execute_ST();
        break;
    case LDR:
        execute_LDR();
        break;
    case STR:
        execute_STR();
        break;
    }
}

void bubble()
{
    IR = NOP;
}

void execute_ADD()
{
    srcnum.word = reg_file[execute_input.r_c][execute_input.s_c];
    dstnum.word = reg_file[0][execute_input.dest];

    if (execute_input.w_b)
    {
        result.byte[LSB] = (dstnum.byte[LSB] + srcnum.byte[LSB]) & LOWBYTE_MASK;
        result.byte[MSB] = dstnum.byte[MSB];
        update_psw(srcnum.byte[LSB], dstnum.byte[LSB], result.byte[LSB], execute_input.w_b);
    }
    else
    {
        result.word = dstnum.word + srcnum.word;
        update_psw(srcnum.word, dstnum.word, result.word, execute_input.w_b);
    }
    reg_file[0][execute_input.dest] = result.word;
}

void execute_ADDC()
{

    srcnum.word = reg_file[execute_input.r_c][execute_input.s_c];
    dstnum.word = reg_file[0][execute_input.dest];

    if (execute_input.w_b)
    {
        result.byte[LSB] = (dstnum.byte[LSB] + srcnum.byte[LSB] + PSW.C) & LOWBYTE_MASK;
        result.byte[MSB] = dstnum.byte[MSB];
        //Update PSW bit
        update_psw(srcnum.byte[LSB] + PSW.C, dstnum.byte[LSB], result.byte[LSB], execute_input.w_b);
    }
    else
    {
        result.word = dstnum.word + srcnum.word + PSW.C;
        update_psw(srcnum.word + PSW.C, dstnum.word, result.word, execute_input.w_b);
    }
    reg_file[0][execute_input.dest] = result.word;
}

void execute_SUB()
{

    srcnum.word = reg_file[execute_input.r_c][execute_input.s_c];
    dstnum.word = reg_file[0][execute_input.dest];

    if (execute_input.w_b)
    {
        result.byte[LSB] = (~srcnum.byte[LSB] + 1 + dstnum.byte[LSB]) & LOWBYTE_MASK;
        result.byte[MSB] = dstnum.byte[MSB];
        update_psw(~srcnum.byte[LSB], dstnum.byte[LSB], result.byte[LSB], execute_input.w_b);
    }
    else
    {
        result.word = ~srcnum.word + 1 + dstnum.word;
        update_psw(~srcnum.word, dstnum.word, result.word, execute_input.w_b);
    }
    reg_file[0][execute_input.dest] = result.word;
}


void execute_SUBC()
{
    srcnum.word = reg_file[execute_input.r_c][execute_input.s_c];
    dstnum.word = reg_file[0][execute_input.dest];

    if (execute_input.w_b)
    {
        result.byte[LSB] = (dstnum.byte[LSB] + (~srcnum.byte[LSB]) + PSW.C) & LOWBYTE_MASK;
        result.byte[MSB] = dstnum.byte[MSB];
        update_psw(~srcnum.byte[LSB] + PSW.C, dstnum.byte[LSB], result.byte[LSB], execute_input.w_b);
    }
    else
    {
        result.word = dstnum.word + (~srcnum.word) + PSW.C;
        update_psw(~srcnum.word + PSW.C, dstnum.word, result.word, execute_input.w_b);
    }
    reg_file[0][execute_input.dest] = result.word;
}

void execute_DADD()
{
    union bcd_word_nibble srcnibble, dstnibble;

    srcnibble.word = reg_file[execute_input.r_c][execute_input.s_c];
    dstnibble.word = reg_file[0][execute_input.dest]; 

    // Perform BCD addition on each nibble
    dstnibble.nibble.nib0 = bcd_add(srcnibble.nibble.nib0, dstnibble.nibble.nib0);
    dstnibble.nibble.nib1 = bcd_add(srcnibble.nibble.nib1, dstnibble.nibble.nib1);

    if (execute_input.w_b == WORD) {
        dstnibble.nibble.nib2 = bcd_add(srcnibble.nibble.nib2, dstnibble.nibble.nib2);
        dstnibble.nibble.nib3 = bcd_add(srcnibble.nibble.nib3, dstnibble.nibble.nib3);
    }

    result.word = dstnibble.word;
    update_psw(srcnibble.word, dstnibble.word, result.word, execute_input.w_b);
    reg_file[0][execute_input.dest] = result.word;
}

void execute_CMP()
{

    srcnum.word = reg_file[execute_input.r_c][execute_input.s_c];
    dstnum.word = reg_file[0][execute_input.dest];

    if (execute_input.w_b)
    {
        result.byte[LSB] = dstnum.byte[LSB] + (~srcnum.byte[LSB] + 1); // Two's compliment
        update_psw((~srcnum.byte[LSB] + 1), dstnum.byte[LSB], result.byte[LSB], execute_input.w_b);
    }
    else
    {
        result.word = dstnum.word + (~srcnum.word + 1);
        update_psw((~srcnum.word + 1), dstnum.word, result.word, execute_input.w_b);
    }
}

void execute_XOR()
{

    srcnum.word = reg_file[execute_input.r_c][execute_input.s_c];
    dstnum.word = reg_file[0][execute_input.dest];

    if (execute_input.w_b)
    {
        dstnum.byte[LSB] ^= srcnum.byte[LSB];
        update_psw1(dstnum.byte[LSB], execute_input.w_b);
        reg_file[0][execute_input.dest] = dstnum.byte[LSB];
    }
    else
    {
        dstnum.word ^= srcnum.word;
        update_psw1(dstnum.word, execute_input.w_b);
        reg_file[REG][execute_input.dest] = dstnum.word;
    }

}

void execute_AND()
{

	if (execute_input.w_b)
    {
        dstnum.byte[LSB] = srcnum.byte[LSB] & dstnum.byte[LSB];
        update_psw1(dstnum.byte[LSB], execute_input.w_b);
        reg_file[REG][execute_input.dest] = dstnum.byte[LSB];
    }
	else
    {
        dstnum.word = srcnum.word & dstnum.word;
        update_psw1(dstnum.word, execute_input.w_b);
        reg_file[REG][execute_input.dest] = dstnum.word;
    }

}


void execute_OR()
{
 
    srcnum.word = reg_file[execute_input.r_c][execute_input.s_c];
    dstnum.word = reg_file[0][execute_input.dest];

    if (execute_input.w_b)
    {
        dstnum.byte[LSB] |= srcnum.byte[LSB];
        update_psw1(dstnum.byte[LSB], execute_input.w_b);
        reg_file[REG][execute_input.dest] = dstnum.byte[LSB];
    }
    else
    {
        dstnum.word |= srcnum.word;
        update_psw1(dstnum.word, execute_input.w_b);
        reg_file[REG][execute_input.dest] = dstnum.word;
    }

}

void execute_BIT()
{

    srcnum.word = reg_file[execute_input.r_c][execute_input.s_c];
    dstnum.word = reg_file[0][execute_input.dest];

    if (execute_input.w_b)
    {
        result.byte[LSB] = dstnum.byte[LSB] & srcnum.byte[LSB];
        update_psw2(result.byte);
        reg_file[REG][execute_input.dest] = result.byte[LSB];

    }
    else
    {
        result.word = dstnum.word & srcnum.word;
        update_psw2(result.word);
        reg_file[REG][execute_input.dest] = result.word;

    }

}

void execute_BIC()
{
 
    srcnum.word = reg_file[execute_input.r_c][execute_input.s_c];
    dstnum.word = reg_file[0][execute_input.dest];

    if (execute_input.w_b)
    {
        dstnum.byte[LSB] &= ~srcnum.byte[LSB];
        update_psw1(dstnum.byte[LSB], execute_input.w_b);
        reg_file[REG][execute_input.dest] = dstnum.byte[LSB];
    }
    else
    {
        dstnum.word &= ~srcnum.word;
        update_psw1(dstnum.word, execute_input.w_b);
        reg_file[REG][execute_input.dest] = dstnum.word;
    }

}

void execute_BIS()
{
    srcnum.word = reg_file[execute_input.r_c][execute_input.s_c];
    dstnum.word = reg_file[0][execute_input.dest];

    if (execute_input.w_b)
    {
        dstnum.byte[LSB] |= srcnum.byte[LSB];
        update_psw1(dstnum.byte[LSB], execute_input.w_b);
        reg_file[REG][execute_input.dest] = dstnum.byte[LSB];
        
    }
    else
    {
        dstnum.word |= srcnum.word;
        update_psw1(dstnum.word, execute_input.w_b);
        reg_file[REG][execute_input.dest] = dstnum.word;
    }

}


void execute_MOV()
{
    // Read the source value from the register file
    srcnum.word = reg_file[execute_input.r_c][execute_input.s_c];

    if (execute_input.w_b)
    {
        // For byte operation, only update the lower byte of the destination register
        reg_file[0][execute_input.dest] = ((reg_file[0][execute_input.dest] & 0xFF00) | (srcnum.byte[LSB] & 0x00FF));
    }
    else
    {
        // For word operation, update the entire destination register
        reg_file[0][execute_input.dest] = srcnum.word;
    }

    if (execute_input.dest == 7)
        bubble();
}

void execute_SWAP()
{
    unsigned short temp;

        // Swap full word
        temp = reg_file[0][execute_input.dest];
        reg_file[REG][execute_input.dest] = reg_file[0][execute_input.s_c];
        reg_file[REG][execute_input.s_c] = temp;
}

void execute_SRA()
{
    dstnum.word = reg_file[0][execute_input.dest];

    if (execute_input.w_b) {
        dstnum.byte[LSB] >>= 1;
        update_psw1(dstnum.byte[LSB], execute_input.w_b);
        reg_file[REG][execute_input.dest] = (reg_file[0][execute_input.dest] & 0xFF00) | (dstnum.byte[LSB] & 0x00FF);
    } else {
        dstnum.word >>= ONE;
        update_psw1(dstnum.word, execute_input.w_b);
        reg_file[0][execute_input.dest] = dstnum.word;
    }

}

void execute_RRC()
{
    unsigned char tempCarry = PSW.C;
    dstnum.word = reg_file[0][execute_input.dest];

    if (execute_input.w_b) 
    {
        PSW.C = dstnum.byte[LSB] & 0x01;
        dstnum.byte[LSB] = (dstnum.byte[LSB] >> 1) | (tempCarry << 7);
        update_psw1(dstnum.byte[LSB], execute_input.w_b);
        reg_file[REG][execute_input.dest] = (reg_file[0][execute_input.dest] & 0xFF00) | (dstnum.byte[LSB] & 0x00FF);
    } 
    else 
    {
        PSW.C = dstnum.word & 0x0001;
        dstnum.word = (dstnum.word >> 1) | (tempCarry << 15);
        update_psw1(dstnum.word, execute_input.w_b);
        reg_file[REG][execute_input.dest] = dstnum.word;
    }
}
void execute_SWPB()
{
    dstnum.word = reg_file[0][execute_input.dest];
    unsigned char temp = dstnum.byte[LSB];
    dstnum.byte[LSB] = dstnum.byte[MSB];
    dstnum.byte[MSB] = temp;
    reg_file[0][execute_input.dest] = dstnum.word;
    update_psw1(dstnum.word, WORD);

}

void execute_SXT()
{

    dstnum.word = reg_file[0][execute_input.dest];

    if (dstnum.byte[LSB] & 0x80) {
        dstnum.word = dstnum.byte[LSB] | 0xFF00; // Sign extend negative number
    } else {
        dstnum.word = dstnum.byte[LSB] & 0x00FF; // Clear upper byte for positive number
    }

    update_psw1(dstnum.word, WORD);
    reg_file[0][execute_input.dest] = dstnum.word;

}

void execute_MOVL()
{
    dstnum.word = reg_file[0][execute_input.dest];  // Current value in destination register

    // Update only the lower byte of the destination register
    reg_file[0][execute_input.dest] = (dstnum.word & 0xFF00) | (execute_input.data & 0x00FF);
}

void execute_MOVLZ()
{

    // Update the destination register with zero-extended immediate value
    reg_file[0][execute_input.dest] = (execute_input.data & 0x00FF);
}

void execute_MOVLS()
{
    // Update the destination register with sign-extended immediate value
    reg_file[0][execute_input.dest] = (execute_input.data & 0x00FF) | 0xFF00;
}

void execute_MOVH()
{
    dstnum.word = reg_file[0][execute_input.dest];  // Current value in destination register

    // Update only the upper byte of the destination register
    reg_file[0][execute_input.dest] = (dstnum.word & 0x00FF) | (execute_input.data << 8);
}

void execute_SETCC()
{
    PSW.V = SETCC.V ? SET : PSW.V;
    PSW.SLP = SETCC.SLP ? SET : PSW.SLP;
    PSW.N = SETCC.N ? SET : PSW.N;
    PSW.Z = SETCC.Z ? SET : PSW.Z;
    PSW.C = SETCC.C ? SET : PSW.C;
}

void execute_CLRCC()
{
    PSW.V = CLRCC.V ? CLEAR : PSW.V;
    PSW.SLP = CLRCC.SLP ? CLEAR : PSW.SLP;
    PSW.N = CLRCC.N ? CLEAR : PSW.N;
    PSW.Z = CLRCC.Z ? CLEAR : PSW.Z;
    PSW.C = CLRCC.C ? CLEAR : PSW.C;
}

void Calculate_LD_Indexed()
{
    offset = offset_table[execute_input.DEC][execute_input.INC][execute_input.w_b];
    switch (execute_input.PRPO)
    {
    case 1:
        reg_file[REG][execute_input.s_c] += offset;
        EA = reg_file[REG][execute_input.s_c];
        DMAR = EA;
        DCTRL = READ;
        break;
    case 0:
        EA = reg_file[REG][execute_input.s_c];
        DMAR = EA;
        DCTRL = READ;
    }
}

void Calculate_ST_Indexed()
{
    offset = offset_table[execute_input.DEC][execute_input.INC][execute_input.w_b];
    switch (execute_input.PRPO)
    {
    case 1:
        reg_file[REG][execute_input.dest] += offset;
        EA = reg_file[REG][execute_input.dest];
        DMAR = EA;
        DCTRL = WRITE;
        break;
    case 0:
        EA = reg_file[REG][execute_input.dest];
        DMAR = EA;
        DCTRL = WRITE;
        /* 
        Then in a function called in E1, the function is going to give DMEM, DMBR:
        DMEM[DMAR] = DMBR. Note this is the word implementation, the byte would need
        the LSB of the DMBR (use union scrnum or dstnum).
        */
    }
}

void Calculate_LDR()
{
    if (execute_input.w_b) // If it's a byte
        EA = reg_file[REG][execute_input.s_c] + execute_input.OFF;
    else // If it's a word
        EA = reg_file[REG][execute_input.s_c] + execute_input.OFF & 0xFFFE;
    DMAR = EA;
    DCTRL = READ;
}

void Calculate_STR()
{
    if (execute_input.w_b) // If it's a byte
        EA = reg_file[REG][execute_input.dest] + execute_input.OFF;
    else // If it's a word
        EA = (reg_file[REG][execute_input.dest] + execute_input.OFF) & 0xFFFE;
    DMAR = EA;
    DCTRL = WRITE;
}

void execute_LD()
{
    if (execute_input.w_b)
    {
        DMBR = DMEM.bytmem[DMAR];
        switch (execute_input.PRPO)
        {
        case 1:
            reg_file[REG][execute_input.dest] = DMBR & LOWBYTE_MASK;
            break;
        case 0:
            reg_file[REG][execute_input.dest] = DMBR & LOWBYTE_MASK;
            reg_file[REG][execute_input.s_c] += offset;
            break;
        }
    }
    else
    {
        DMBR = DMEM.wrdmem[DMAR>>ONE];
        switch (execute_input.PRPO)
        {
        case 1:
            reg_file[REG][execute_input.dest] = DMBR;
            break;
        case 0:
            reg_file[REG][execute_input.dest] = DMBR;
            reg_file[REG][execute_input.s_c] += offset;
            break;
        }
    }
}

void execute_ST()
{
    if (execute_input.w_b)
    {
        DMBR = reg_file[REG][execute_input.s_c];
        switch (execute_input.PRPO)
        {
        case 1:
            DMEM.bytmem[DMAR] = DMBR & LOWBYTE_MASK;
            break;
        case 0:
            DMEM.bytmem[DMAR] = DMBR & LOWBYTE_MASK;
            reg_file[REG][execute_input.dest] += offset;
            break;
        }
    }
    else
    {
        DMBR = reg_file[REG][execute_input.s_c];
        switch (execute_input.PRPO)
        {
        case 1:
            DMEM.wrdmem[DMAR>>ONE] = DMBR;
            break;
        case 0:
            DMEM.wrdmem[DMAR>>ONE] = DMBR;
            reg_file[REG][execute_input.dest] += offset;
            break;
        }
    }
}

void execute_LDR()
{
    /*
    You load from a memory location to a register
    V1 equ $2
     i.e. LDR R0,V1,R1
     R1 <- mem[R0 + V1(2)]
     
     */
    
    if (execute_input.w_b)
    {
        DMBR = DMEM.bytmem[DMAR];
        reg_file[REG][execute_input.dest] = DMBR & LOWBYTE_MASK;
    }
    else
    {
        DMBR = DMEM.wrdmem[DMAR>>ONE];
        reg_file[REG][execute_input.dest] = DMBR;
    }
}

void execute_STR()
{
    if (execute_input.w_b)
        DMEM.bytmem[DMAR] = reg_file[REG][execute_input.s_c] & LOWBYTE_MASK;
    else
        DMEM.wrdmem[DMAR>>ONE] = reg_file[REG][execute_input.s_c];
}

void execute_BL()
{
    LR = PC - TWO;
    PC = PC - 2 + execute_input.OFF_13bit;
    bubble();
    CEX_state = FALSE; // In the requirements from A5, we know that BL disables the CEX command
}

void execute_BEQ_BZ()
{
    if (PSW.Z)
    {
        PC = PC - 2 + execute_input.OFF_10bit;
        bubble();
    }
}

void execute_BNE_BNZ()
{
    if (!PSW.Z)
    {
        PC = PC - 2 + execute_input.OFF_10bit;
        bubble();
    }
}

void execute_BC_BHS()
{
    if (PSW.C)
    {
        PC = PC - 2 + execute_input.OFF_10bit;
        bubble();
    }
}

void execute_BNC_BLO()
{
    if (!PSW.C)
    {
        PC = PC - 2 + execute_input.OFF_10bit;
        bubble();
    }
}

void execute_BN()
{
    if (PSW.N)
    {
        PC = PC - 2 + execute_input.OFF_10bit;
        bubble();
    }
}

void execute_BGE()
{
    if (PSW.N == PSW.V)
    {
        PC = PC - 2 + execute_input.OFF_10bit;
        bubble();
    }
}

void execute_BLT()
{
    if (PSW.N != PSW.V)
    {
        PC = PC - 2 + execute_input.OFF_10bit;
        bubble();
    }
}

void execute_BRA()
{
    PC = PC - 2 + execute_input.OFF_10bit;
    bubble();
}

void execute_CEX()
{/*
 This function is responsible for executing the CEX instruction

 - First it checks the decoded Condition Prefix.
   Determining which one of the PSW bits to be tested,
   finding out if it/they are true then returns the status
   to a bool variable.
 */ 
        switch (Cond_prefix)
        {
            // This is the switch case to evaluate if the condition is true or false based of the PSW bits
        case EQ:
            CEX_test = (PSW.Z == SET) ? TRUE : FALSE;
            break;
        case NE:
            CEX_test = (PSW.Z == CLEAR) ? TRUE : FALSE;
            break;
        case CS:
            CEX_test = (PSW.C == SET) ? TRUE : FALSE;
            break;
        case CC:
            CEX_test = (PSW.C == CLEAR) ? TRUE : FALSE;
            break;
        case MI:
            CEX_test = (PSW.N == SET) ? TRUE : FALSE;
            break;
        case PL:
            CEX_test = (PSW.N == CLEAR) ? TRUE : FALSE;
            break;
        case VS:
            CEX_test = (PSW.V == SET) ? TRUE : FALSE;
            break;
        case VC:
            CEX_test = (PSW.V == CLEAR) ? TRUE : FALSE;
            break;
        case HI:
            CEX_test = ((PSW.C == SET) && (PSW.Z == CLEAR)) ? TRUE : FALSE;
            break;
        case LS:
            CEX_test = ((PSW.C == CLEAR) || (PSW.Z == SET)) ? TRUE : FALSE;
            break;
        case GE:
            CEX_test = (PSW.N == PSW.V) ? TRUE : FALSE;
            break;
        case LT:
            CEX_test = (PSW.N != PSW.V) ? TRUE : FALSE;
            break;
        case GT:
            CEX_test = ((PSW.Z == CLEAR) && (PSW.N == PSW.V)) ? TRUE : FALSE;
            break;
        case LE:
            CEX_test = ((PSW.Z == SET) || (PSW.N != PSW.V)) ? TRUE : FALSE;
            break;
        case TR:
            CEX_test = TRUE;
            break;
        case FL:
            CEX_test = FALSE;
            break;
        }

        CEX_state = TRUE;// Enabling the CEX state ~ CEX states acts like a flag. It tells the emulator to go into conditional execution mode.
}

void CEX_checker()
// This function is constantly polling if the CEX state is set
// Checking if the instructions to executed are executed, and bubbles the ones to not be executed
{
    if (CEX_test)
        // If the decoded condition is true, execute the amount of true statements
    {
        if (CEX_truecount)
            CEX_truecount--; // If the condition is true, decrement the count of true instructions and execute each one
        else if (!CEX_truecount && CEX_falsecount)
        {
            bubble();// If all true conditions have been executed, start bubbling false ones decrementing the count of false ones
            CEX_falsecount--; 
        }
    }
    else // The decoded condition must be false. Bubble true and execute false
    {
        if (CEX_truecount)
        {
            bubble();
            CEX_truecount--; // bubble all the true instructions, decrementing the count of the overall true conditions
        }
            
        else if (CEX_falsecount && !CEX_truecount)
            CEX_falsecount--; // If the count of all true conditions is zero, now execute the false ones
    }

    if (!CEX_truecount && !CEX_falsecount)
        CEX_state = FALSE;
    /*
        We exit the CEX command under two conditions :
        - if all true conditions are executed and all false are bubbled
        or
        - if all true are bubbled and false are executed
        Which is what the if statement above is
    */
}

void update_psw(unsigned short src, unsigned short dest, unsigned short result, unsigned short wb)
{
    unsigned short MSsrc, MSdest, MSres;

    if (wb == 0)
    {
        MSsrc = src >> 15;
        MSdest = dest >> 15;
        MSres = result >> 15;
    }

    else
    {
        MSsrc = src >> 7;
        MSdest = dest >> 7;
        MSres = result >> 7;
    }

    PSW.C = carry[MSsrc][MSdest][MSres]; // Obtain carry bit from the carrytable
    PSW.V = overflow[MSsrc][MSdest][MSres];// Obtain overflow bit from the overflow table
    PSW.N = (MSres == 1);// Set if the MSB of the result is '1', and clear otherwise 
    PSW.Z = (result == 0);// Set if the result is a zero value and clear if it's a non-zero value
}

void update_psw1(unsigned short result, unsigned short wb)
{
    unsigned short MSres;

    if (wb == 0)
        MSres = result >> 15;
    else
        MSres = result >> 7;

    PSW.N = (MSres == 1);// Set if the MSB of the result is '1', and clear otherwise 
    PSW.Z = (result == 0);// Set if the result is a zero value and clear if it's a non-zero value
}

void update_psw2(unsigned short result)
{
    PSW.Z = (result == 0);// Set if the result is a zero value and clear if it's a non-zero value
}

//void update_psw3(unsigned short dest, unsigned short wb)

unsigned short bcd_add(unsigned short nibble_A, unsigned short nibble_B) 
{
    unsigned short temp_res = nibble_A + nibble_B + PSW.C;
    if (temp_res >= 10) 
    {
        temp_res -= 10;
        PSW.C = 1;
    }
    else 
    {
        PSW.C = 0;
    }
    return temp_res;
}

void Run_pipeline_continuous() 
{// IMAR becomes zero after every odd clock tick why??

    ctrl_c_fnd = FALSE;
    signal(SIGINT, (_crt_signal_t)sigint_hdlr);
    printf("Clock\tPC\tInstruction\tFetch\t\tDecode\t\tExecute\t\tPSW\n");
    while (BKPNT_CHECK != breakpoint && !ctrl_c_fnd)
    {
        if (ZERO_CLK)
        {
            IR = NOP;
            //printf("Fetching NOP\n");
        }
        
        
        if (EVEN_CLK)
        {
            if (execute_input.UI == LD || execute_input.UI == ST || execute_input.UI == LDR || execute_input.UI == STR)
            E1();
            F0();
            D0();
            printf("%d\t%04X\t%04X\t\tF0: %04X\tD0: %04X\n", Clock, IMAR, IMBR, IMAR, IR);
        }
        else
        {
            F1();
            E0();
            if (CEX_state) // CEX_state only turns on if CEX was decoded and executed. If a BL is detected, the CEX_state is disabled.
                CEX_checker();
            printf("%d\t\t\t\tF1: %04X\t\t\tE0: %04X", Clock, IMBR, execute_input.opcode);
            printf("\t%x%x%x%x \n", PSW.V, PSW.N, PSW.Z, PSW.C);
        }
        Clock++;
    }
}

void Run_pipeline_single()
{
    printf("Clock\tPC\tInstruction\tFetch\t\tDecode\t\tExecute\t\tPSW\n");
    if (ZERO_CLK)
    {
        IR = NOP;
        //printf("Fetching NOP\n");
    }

        
    if (EVEN_CLK)
    {
        if (execute_input.UI == LD || execute_input.UI == ST || execute_input.UI == LDR || execute_input.UI == STR)
        E1();
        F0();
        D0();
        printf("%d\t%04X\t%04X\t\tF0: %04X\tD0: %04X\n", Clock, IMAR, IMBR, IMAR, IR);
    }
    else
    {
        F1();
        E0();
        if (CEX_state) // CEX_state only turns on if CEX was decoded and executed. If a BL is detected, the CEX_state is disabled.
            CEX_checker();
        printf("%d\t\t\t\tF1: %04X\t\t\tE0: %04X", Clock, IMBR, execute_input.opcode);
        printf("\t%x%x%x%x \n", PSW.V, PSW.N, PSW.Z, PSW.C);
    }

    Clock++;
}

void sigint_hdlr()
{
    /*
    - Invoked when SIGINT (control-C) is detected
    - changes state of waiting_for_signal
    - signal must be reinitialized
    */
    ctrl_c_fnd = TRUE;
    signal(SIGINT, (_crt_signal_t)sigint_hdlr); /* Reinitialize SIGINT */
}
