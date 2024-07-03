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
bool ICTRL;
unsigned short ;
ProgramStatusWord PSW;

unsigned carry[2][2][2] = { 0, 0, 1, 0, 1, 0, 1, 1 };
unsigned overflow[2][2][2] = { 0, 1, 0, 0, 0, 0, 1, 0 };

union w_b srcnum, dstnum, result;

void F0()
{
	IMAR = PC;
	ICTRL = 0; // Signal to RD word


	PC += 2;
    reg_file[0][7] = PC; // Giving register 7 the value of PC. To update R7 and for debugging purposes

    printf("IMAR -> %x\n", IMAR);
    printf("Start: PC: %04X ", PC);
    printf("PSW: %x%x%x%x ", PSW.V, PSW.N, PSW.Z, PSW.C);
    printf("Brkpnt: %04X ", breakpoint);
    printf("Clk: %d\n", Clock);
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
     if (((command.opcode >> 8) & OPCODE_MASK_40) == OPCODE_GROUP_40)// If it falls between the 0x40(0100 0000) range call this function
         handle_group_40(command);// Calling the function passing command, so the starting address of the IMEM can be obtained by handle_group_40
     else if (((command.opcode >> 11) & OPCODE_MASK_MOV) == OPCODE_MASK_MOV)
         handle_group_MOV(command);
     else if (command.opcode == OPCODE_NO_OPERATION)
     {
         printf("Program is now ending\n");
         execute_input.UI = 0;
         return;
     }
     else
         printf("%04X: %04X\n", command.address, command.opcode);// Upon if the instruction is unidentified

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
    default :
        printf("Instruction % 04X: % 04X not executed or End of Program\n"
            , execute_input.address, execute_input.opcode);
    }
    
}

void execute_ADD()
{
    srcnum.word = reg_file[execute_input.r_c][execute_input.s_c];
    dstnum.word = reg_file[0][execute_input.dest];

    if (execute_input.w_b)
    {
        result.byte[LSB] = (dstnum.byte[LSB] + srcnum.byte[LSB]) & LOWBYTES;
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
        result.byte[LSB] = (dstnum.byte[LSB] + srcnum.byte[LSB] + PSW.C) & LOWBYTES;
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
        result.byte[LSB] = (~srcnum.byte[LSB] + 1 + dstnum.byte[LSB]) & LOWBYTES;
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
        result.byte[LSB] = (dstnum.byte[LSB] + (~srcnum.byte[LSB]) + PSW.C) & LOWBYTES;
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
        reg_file[0][execute_input.dest] = result.byte[LSB];
    }
    else
    {
        result.word = dstnum.word + (~srcnum.word + 1);
        update_psw((~srcnum.word + 1), dstnum.word, result.word, execute_input.w_b);
        reg_file[0][execute_input.dest] = result.word;
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
        reg_file[0][execute_input.dest] = dstnum.word;
    }

}

void execute_AND()
{

	if (execute_input.w_b)
    {
        dstnum.byte[LSB] = srcnum.byte[LSB] & dstnum.byte[LSB];
        update_psw1(dstnum.byte[LSB], execute_input.w_b);
        reg_file[0][execute_input.dest] = dstnum.byte[LSB];
    }
	else
    {
        dstnum.word = srcnum.word & dstnum.word;
        update_psw1(dstnum.word, execute_input.w_b);
        reg_file[0][execute_input.dest] = dstnum.word;
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
        reg_file[0][execute_input.dest] = dstnum.byte[LSB];
    }
    else
    {
        dstnum.word |= srcnum.word;
        update_psw1(dstnum.word, execute_input.w_b);
        reg_file[0][execute_input.dest] = dstnum.word;
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
        reg_file[0][execute_input.dest] = result.byte[LSB];

    }
    else
    {
        result.word = dstnum.word & srcnum.word;
        update_psw2(result.word);
        reg_file[0][execute_input.dest] = result.word;

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
        reg_file[0][execute_input.dest] = dstnum.byte[LSB];
    }
    else
    {
        dstnum.word &= ~srcnum.word;
        update_psw1(dstnum.word, execute_input.w_b);
        reg_file[0][execute_input.dest] = dstnum.word;
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
        reg_file[0][execute_input.dest] = dstnum.byte[LSB];
        
    }
    else
    {
        dstnum.word |= srcnum.word;
        update_psw1(dstnum.word, execute_input.w_b);
        reg_file[0][execute_input.dest] = dstnum.word;
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
}

void execute_SWAP()
{
    unsigned short temp;

        // Swap full word
        temp = reg_file[0][execute_input.dest];
        reg_file[0][execute_input.dest] = reg_file[execute_input.r_c][execute_input.s_c];
        reg_file[execute_input.r_c][execute_input.s_c] = temp;
}

void execute_SRA()
{
    dstnum.word = reg_file[0][execute_input.dest];

    if (execute_input.w_b) {
        dstnum.byte[LSB] >>= 1;
        update_psw1(dstnum.byte[LSB], execute_input.w_b);
        reg_file[0][execute_input.dest] = (reg_file[0][execute_input.dest] & 0xFF00) | (dstnum.byte[LSB] & 0x00FF);
    } else {
        dstnum.word >>= 1;
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
        reg_file[0][execute_input.dest] = (reg_file[0][execute_input.dest] & 0xFF00) | (dstnum.byte[LSB] & 0x00FF);
    } 
    else 
    {
        PSW.C = dstnum.word & 0x0001;
        dstnum.word = (dstnum.word >> 1) | (tempCarry << 15);
        update_psw1(dstnum.word, execute_input.w_b);
        reg_file[0][execute_input.dest] = dstnum.word;
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
    while (BKPNT_CHECK != breakpoint)
    {
        if (ZERO_CLK)
        {
            IR = NOP;
            printf("Fetching NOP\n");
        }
        
        
        if (EVEN_CLK)
        {
            printf("Stages: F0 D0\n");
            F0();
            D0();
        }
        else
        {
            printf("Stages: F1 E0\n");
            F1();
            E0();
            printf("End: PC: %04X Clk: %d ", PC, Clock);
            printf("PSW: %x%x%x%x \n\n", PSW.V, PSW.N, PSW.Z, PSW.C);
        }
        
        Clock++;
    }

}

void Run_pipeline_single()
{
        if (ZERO_CLK)
        {
            IR = NOP;
            printf("Fetching NOP\n");
        }

        
        if (EVEN_CLK)
        {
            printf("Stages: F0 D0\n");
            F0();
            D0();
        }
        else
        {
            printf("Stages: F1 E0\n");
            F1();
            E0();
            printf("End: PC: %04X Clk: %d ", PC, Clock);
            printf("PSW: %x%x%x%x \n\n", PSW.V, PSW.N, PSW.Z, PSW.C);
        }

    Clock++;
}

