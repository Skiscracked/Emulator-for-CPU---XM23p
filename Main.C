/*
 Name: Semilore Kayode
 B00863886
 Purpose: Creating a loader for the pipelined XM23. This is the Main file.
*/
#include "Loader.h"

starting_address = 0; // Initializing the external variable from my header file, ensuring the PC(program counter is initialized)
IMEM_SA = 0; // Initializing IMEM_SA to 0 before using it
PC = 0; // Initializing PC to 0

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
        printf("\n? - Lists the options (below)\n");
        printf("L - Load\n");
        printf("M - Display mem\n");
        printf("P - Print instructions\n");
        printf("D - Debugger Commands\n");
        printf("X - Exit\n");
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
        case 'P':
            decode_and_display(IMEM_SA);
            break;
        case 'D':
            display_and_process_debug_menu();
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

