#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Memory sizes
#define MEM_SIZE 65536

// Memory arrays
unsigned char IMEM[MEM_SIZE];
unsigned char DMEM[MEM_SIZE];

// Program Counter
unsigned int starting_address = 0;

// Function declarations
void Access_record(FILE* file); //function to open the S-record file
void Parse_record(char* S_record); //function that parses the record, determining which is S0|S1|S2|S9
void Process_header(char* S_record);//function to process the header obtained from S0
void Map_Data_to_IMEM(char* S_record);
void Map_Data_to_DMEM(char* S_record);
void Map_starting_address(char* S_record);
unsigned int extractLength(char* S_record);
unsigned int extractAddress(char* S_record);
void extractData(char* S_record, unsigned int length, unsigned char* data);
void writeDataToMemory(unsigned char* memoryType, unsigned int address, unsigned char* data, unsigned int length);
void setProgramCounter(unsigned int address);
void Display_records();
void load_file();
void list_options();
void Display_IMEM();
void Display_DMEM();

int main() {
    char option;
    char filename[100];

    printf("Enter the filename: ");
    scanf("%s", filename);

    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return 1;
    }

    Access_record(file);
    fclose(file);

    while (1) {
        printf("\n? - lists the options (below)\n");
        printf("L - load\n");
        printf("M - display mem\n");
        printf("X - exit\n");
        printf("Option: ");
        scanf(" %c", &option);

        switch (option) {
        case '?':
            list_options();
            break;
        case 'L':
            load_file();
            break;
        case 'M':
            Display_records();
            break;
        case 'X':
            printf("Exiting...\n");
            return 0;
        default:
            printf("Unknown option. Type '?' for list of options.\n");
            break;
        }
    }

    return 0;
}

void Access_record(FILE* file) {
    char S_record[100];
    while (fgets(S_record, sizeof(S_record), file)) {
        Parse_record(S_record);
    }
}

void Parse_record(char* S_record) {
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
    char header[100];
    sscanf(S_record + 4, "%s", header); // Assuming the header is located after the first 4 characters
    printf("Header processed: %s\n", header);
}

void Map_Data_to_IMEM(char* S_record) {
    unsigned int length = extractLength(S_record);
    unsigned int address = extractAddress(S_record);
    unsigned char data[100];
    extractData(S_record, length, data);
    writeDataToMemory(IMEM, address, data, length);
}

void Map_Data_to_DMEM(char* S_record) {
    unsigned int length = extractLength(S_record);
    unsigned int address = extractAddress(S_record);
    unsigned char data[100];
    extractData(S_record, length, data);
    writeDataToMemory(DMEM, address, data, length);
}

void Map_starting_address(char* S_record) {
    unsigned int address = extractAddress(S_record);
    setProgramCounter(address);
}

unsigned int extractLength(char* S_record) {
    unsigned int length;
    sscanf(S_record + 2, "%2x", &length);
    return length - 3; // Subtracting 3 for address and checksum
}

unsigned int extractAddress(char* S_record) {
    unsigned int address;
    sscanf(S_record + 4, "%4x", &address);
    return address;
}

void extractData(char* S_record, unsigned int length, unsigned char* data) {
    for (unsigned int i = 0; i < length; i++) {
        sscanf(S_record + 8 + i * 2, "%2hhx", &data[i]);
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
    printf("Would you like to print IMEM or DMEM[I/D]?\n");
    scanf(" %c", &choice);

    if (choice == 'I' || choice == 'i')
        Display_IMEM();
    else if (choice == 'D' || choice == 'd')
        Display_DMEM;
    else
        printf("Unknown option. Type '?' for list of options.\n");



}


void load_file() {
    // Implement functionality to load another file
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
    // Display the list of options
    printf("\nOptions:\n");
    printf("L - load\n");
    printf("M - display mem\n");
    printf("X - exit\n");
}

void Display_IMEM() 
{
    int start, stop;
    printf("Displaying IMEM (Instruction Memory):\n");
    printf("Where would you like to start from:");
    scanf("%x", &start);
    printf("Where would you like to stop at:");
    scanf("%x", &stop);
    for (unsigned int address = start; address <= stop; address+=32) 
    {
        for (int j = 0; j < 32; j+=2)
        {
            printf("Address: 0x%04X Value: 0x%02X Char: %c\n", address, IMEM[address + j], IMEM[address + j+1]);
        }
    }
}

void Display_DMEM()
{
    int start, stop;
    printf("Displaying DMEM (Data Memory):\n");
    printf("Where would you like to start from:");
    scanf("%x", &start);
    printf("Where would you like to stop at:");
    scanf("%x", &stop);
    for (unsigned int address = start; address <= stop; address++)
    {
        for (int j = 0; j < 16; j++)
        {
            printf("Address: 0x%04X Value: 0x%02X Char: %c\n", address, DMEM[address + j], DMEM[address + j]);
        }
    }
}