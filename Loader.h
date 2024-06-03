#pragma once
/*
 Name: Semilore Kayode
 B00863866
 Course: ECED 3401
 Prepared for: Dr Hughes
 Purpose: Creating a loader for the pipelined XM23. This is the header file.
*/
#ifndef LOADER_H
#define LOADER_H

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Decoder.h"

//Memory size
#define MEM_SIZE 65536
#define ARRAY_LIMIT 100

//Memory arrays
unsigned char IMEM[MEM_SIZE];
unsigned char DMEM[MEM_SIZE];

//Starting address from S9
extern unsigned int starting_address;



//Function declarations
void Access_record(FILE* file); //Function to open the S-record file
void Parse_record(char* S_record); //Function that parses the record, determining which is S0|S1|S2|S9
void Process_header(char* S_record); //Function to process the header obtained from S0
void Map_Data_to_IMEM(char* S_record);//Function responsible for extracting the information from S1 and passing it to be written to memory
void Map_Data_to_DMEM(char* S_record);//Function responsible for extracting the information from S2 and passing it to be written to memory
void Map_starting_address(char* S_record);//function to map address to set the program counter
unsigned int extractLength(char* S_record);
unsigned int extractAddress(char* S_record);
void extractData(char* S_record, unsigned int length, unsigned char* data);
void writeDataToMemory(unsigned char* memoryType, unsigned int address, unsigned char* data, unsigned int length);//function responsible for writing to memory
void setProgramCounter(unsigned int address);
void Display_records();
void load_file();
void list_options();
void Display_IMEM();
void Display_DMEM();
unsigned int calculateChecksum(char* S_record);
int verifyChecksum(char* S_record);
#endif


