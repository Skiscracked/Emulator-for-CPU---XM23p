/*
 Name: Semilore Kayode
 B00863886
 Purpose: Creating a loader for the pipelined XM23. This is the Implementation file.
 Note: unsigned variables are used to ensure proper handling of binary values
*/
#include "Loader.h"

void Access_record(FILE* file) {
    char S_record[ARRAY_LIMIT];
    while (fgets(S_record, sizeof(S_record), file)) {
        Parse_record(S_record);
    }
}

void Parse_record(char* S_record) {

    if (!verifyChecksum(S_record)) //first check if the checksum from the record and calculated checksum values are in correspondence
    {
        printf("Checksum error in record: %s\n", S_record);
        return;
    }

    char type[3];
    strncpy(type, S_record, 2);
    type[2] = '\0';

    if (strcmp(type, "S0") == 0)
        Process_header(S_record);
    else if (strcmp(type, "S1") == 0)
        Map_Data_to_IMEM(S_record);
    else if (strcmp(type, "S2") == 0)
        Map_Data_to_DMEM(S_record);
    else if (strcmp(type, "S9") == 0)
        Map_starting_address(S_record);
    else
        printf("Unknown record type.\n");
}

void Process_header(char* S_record) {
    char header[ARRAY_LIMIT];
    sscanf(S_record + 4, "%s", header); //Knowing the header is located after the first 4 characters
    printf("Header processed: %s\n", header);
}

void Map_Data_to_IMEM(char* S_record) {
    unsigned int length = extractLength(S_record);//extracting the length from the S-record array
    unsigned int address = extractAddress(S_record);//extracting the address from the S-record array
    IMEM_SA = address;
    unsigned char data[100];
    extractData(S_record, length, data);//extracting data
    writeDataToMemory(IMEM, address, data, length);//writing all 3 extracts to the IMEM array
}

void Map_Data_to_DMEM(char* S_record) {
    unsigned int length = extractLength(S_record);
    unsigned int address = extractAddress(S_record);
    unsigned char data[100];
    extractData(S_record, length, data);
    writeDataToMemory(DMEM, address, data, length);//writing all 3 extracts to the DMEM array
}

void Map_starting_address(char* S_record) {
    unsigned int address = extractAddress(S_record);
    setProgramCounter(address);
}

unsigned int extractLength(char* S_record) {
    unsigned int length;
    sscanf(S_record + 2, "%2x", &length);
    return length - 3; //Subtracting 3 for address and checksum
}

unsigned int extractAddress(char* S_record) {
    unsigned int address;
    sscanf(S_record + 4, "%4x", &address);
    return address;
}

void extractData(char* S_record, unsigned int length, unsigned char* data) {
    for (unsigned int i = 0; i < length; i++) {
        if (sscanf(S_record + 8 + i * 2, "%2hhx", &data[i]) != 1) {
            printf("Error parsing data at position %u\n", i);
            data[i] = 0;  //Setting default value on error
        }
    }
}

void writeDataToMemory(unsigned char* memoryType, unsigned int address, unsigned char* data, unsigned int length) {
    for (unsigned int i = 0; i < length; i++) {
        memoryType[address + i] = data[i];
    }
}

void setProgramCounter(unsigned int address) {
    starting_address = address;
    printf("Program Counter set to start at 0x%04X\n", address);
}

void Display_records() {
    char choice;
    printf("Would you like to print IMEM or DMEM [I/D]?\n");
    scanf(" %c", &choice);

    if (choice == 'I' || choice == 'i')
        Display_IMEM();
    else if (choice == 'D' || choice == 'd')
        Display_DMEM();
    else
        printf("Unknown option. Type '?' for list of options.\n");
}

void load_file() {
    char filename[100];
    printf("Enter the filename to load: ");
    scanf("%s", filename);

    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    Access_record(file);
    fclose(file);
}

void list_options() {
    printf("\nOptions:\n");
    printf("L - load\n");
    printf("M - display mem\n");
    printf("X - exit\n");
}

void Display_IMEM() {
    int start, stop;
    printf("Displaying IMEM (Instruction Memory):\n");
    printf("Where would you like to start from: ");
    scanf("%x", &start);
    printf("Where would you like to stop at: ");
    scanf("%x", &stop);

    for (unsigned int address = start; address <= stop; address += 16) {//Inrementing by 16, because 16 hex is 10 in binary
        printf("%04X: ", address);//Printing out an interval of 10 address ranges
        for (int i = 0; i < 16; i++) {
            if (address + i < MEM_SIZE) {
                printf("%02X ", IMEM[address + i]);
            }
            else {
                printf("   ");
            }
        }
        printf(" ");
        for (int i = 0; i < 16; i++) {
            if (address + i < MEM_SIZE) {
                unsigned char c = IMEM[address + i];
                if (isprint(c)) {
                    printf("%c", c);//printing out the ASCII equivalents of the byte pairs
                }
                else {
                    printf(".");
                }
            }
        }
        printf("\n");
    }
}

void Display_DMEM() {
    int start, stop;
    printf("Displaying DMEM (Data Memory):\n");
    printf("Where would you like to start from: ");
    scanf("%x", &start);
    printf("Where would you like to stop at: ");
    scanf("%x", &stop);

    for (unsigned int address = start; address <= stop; address += 16) {//Inrementing by 16, because 16 hex is 10 in binary
        printf("%04X: ", address);                                      //Printing out an interval of 10 address ranges
        for (int i = 0; i < 16; i++) {
            if (address + i < MEM_SIZE) {
                printf("%02X ", DMEM[address + i]);
            }
            else {
                printf("   ");
            }
        }
        printf(" ");
        for (int i = 0; i < 16; i++) {//printing out ASCII equivalents for byte pairs in DMEM
            if (address + i < MEM_SIZE) {
                unsigned char c = DMEM[address + i];
                if (isprint(c)) {
                    printf("%c", c);
                }
                else {
                    printf(".");
                }
            }
        }
        printf("\n");
    }
}

unsigned int calculateChecksum(char* S_record) {
    unsigned int sum = 0;
    unsigned int byte;
    int len = strlen(S_record);

    //Sum all bytes from the length field to the byte before the checksum
    for (int i = 2; i < len - 3; i += 2) {//It had to be -3 because fgets adds a LF
        sscanf(S_record + i, "%2x", &byte);
        sum += byte;
    }

    //One's complement of the sum
    unsigned int checksum = (~sum) & 0xFF; //Invert all bits
    return checksum;
}


int verifyChecksum(char* S_record) {
    unsigned int recordChecksum;
    unsigned int calculatedChecksum;

    //Extract the checksum byte from the S-record
    sscanf(S_record + strlen(S_record) - 3, "%2x", &recordChecksum);//It had to be -3 because fgets adds a LF

    //Calculate the checksum based on the rest of the S-record
    calculatedChecksum = calculateChecksum(S_record);

    //Verify if the calculated checksum matches the extracted checksum
    return recordChecksum == calculatedChecksum;
}

//Function to fetch - obtain and return the instruction
Instruction fetch(unsigned int pc) {
    Instruction instr;
    instr.opcode = (IMEM[pc + 1] << 8) | IMEM[pc];
    instr.address = pc;
    return instr;
}

void decode_and_display(unsigned int SA)//this function is getting the starting address of the IMEM array, to be able to print instructions from that spot
{
    Instruction command = fetch(SA);
    if (((command.opcode >> 8) & 0xF0) == 0x40)//if it falls between the 0x40(0100 0000) range call this function
        handle_group_40(command);//calling the function passing command, so the starting address of the IMEM can be obtained by handle_group_40
}//handle the next edgecases for the other opcodes.

void handle_group_40(Instruction instr) //addy in this case is going to be the content of the PC.
//The PC is going to hold the next instructions
{
    unsigned int addy = instr.address;
    switch ((instr.opcode >> 8) & 0x000F)  //Masking to get the relevant bits (getting the lower 4 bits of the first 8 bit from the 16 bits) 
    {
    case 0x00: //ADD opcode
        printf("%04X: ADD\n", addy);//ADD Instruction
        break;
    case 0x01:
        printf("%04X: ADDC", addy);//ADDC Instruction
        break;
    case 0x02:
        printf("%04X: SUB", addy);//SUB Instruction
        break;
    case 0x03:
        printf("%04X: SUBC", addy);//SUBC Instruction
        break;
    case 0x04:
        printf("%04X: DADD", addy);//DADD Instruction
        break;
    case 0x05:
        printf("%04X: CMP", addy);//CMP Instruction
        break;
    case 0x06:
        printf("%04X: XOR", addy);//XOR Instruction
        break;
    case 0x07:
        printf("%04X: AND", addy);//AND Instruction
        break;
    case 0x08:
        printf("%04X: OR", addy);//OR Instruction
        break;
    case 0x09:
        printf("%04X: BIT", addy);//BIT Instruction
        break;
    case 0x0A:
        printf("%04X: BIC", addy);//BIC Instruction
        break;
    case 0x0B:
        printf("%04X: BIS", addy);//BIS Instruction
        break;
    }

}

void handle_group_4C(Instruction instr, unsigned int addy)
{
    switch ((instr.opcode >> 7) & 0x03)//Shifting and masking 
    {
    case 0x00:
        printf("%04X: MOV", addy); //MOV Instruction
        break;
    case 0x01:
        printf("%04X: SWAP", addy);//SWAP Instruction
        break;
    }
}

void handle_group_132(Instruction instr, unsigned int addy)
{
    switch ((instr.opcode >> 3) & 0x01)
    {
    case 0x00:
        printf("%04X: SRA", addy); //SRA Instruction
        break;
    case 0x01:
        printf("%04X: RRC", addy);//RRC Instruction
        break;
    }
}

void handle_group_9A4(Instruction instr, unsigned int addy)
{
    switch ((instr.opcode >> 3) & 0x000F)
    {
    case 0x03:
        printf("%04X: SWPB", addy);//SWPB Instruction
        break;
    case 0x04:
        printf("%04X: SXT", addy);//SXT Instruction
        break;
    }
}