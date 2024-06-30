/*
 Name: Semilore Kayode
 B00863886
 Purpose: Creating an emulator for the pipelined XM23. This is the Main file.
 Date: 06/20/2024
*/
#include "Loader.h"

unsigned short starting_address, IMEM_SA, PC;

union mem IMEM = { 0 };
union mem DMEM = { 0 };

int main() {
    
    char option;
    char filename[100];
    bool single_step_mode = false;

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
        printf("D - Debugger Commands\n");
        printf("G - go (continuous or single step [toggle with 'I'])\n");
        printf("I - increment PC (toggle continuous run or single step)\n");
        printf("L - Load\n");
        printf("M - Display mem\n");
        printf("P - Print instructions\n");
 //       printf("S - Set breakpoint\n");
        printf("X - Exit\n");
        printf("Option: ");
        scanf(" %c", &option);

        switch (tolower(option)) {
        case '?':
            list_options();
            break;
        case 'l':
            load_file();
            break;
        case 'm':
            Display_records();
            break;
        case 'P':
            decode_and_display();
            break;
        case 'd':
            display_and_process_debug_menu();
            break;
        case 'g':
            if (single_step_mode) 
                Run_pipeline_single();
            else
                Run_pipeline_continuous();
            break;
        case 'i': // Toggle single-step mode
            single_step_mode = !single_step_mode;
            printf("Single step is %s\n", single_step_mode ? "enabled" : "disabled");
            break;
        case 'x':
            printf("Exiting...\n");
            return 0;
        default:
            printf("Unknown option. Type '?' for list of options.\n");
            break;

        }
    }

    return 0;
}

