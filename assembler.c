 /*======================================================================================================
* FILE        : Team5_assembler.c
* AUTHOR      : Josh Ratificar (Hardware Lead)
*               Ben Cesar Cadungog (Software Lead)
*               Jeddah Laine Luce�ara  (Research Lead)
*               Harold Marvin Comendador (Documentation Lead)
* DESCRIPTION : This program will translate the assembly code into formatted TRACS "C" code.
* COPYRIGHT   : 23 April, 2024
* REVISION HISTORY:
*   21 April, 2024: V1.0 - File Created
*   22 April, 2024: V1.1 - Added the process_file function to read and format the assembly code
*   23 April, 2024, V2.0 - Implemented Logic for label addressing, opcode conversion, and output redirection.
======================================================================================================*/
/*===============================================
 *   HEADER FILES
 *==============================================*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include <stdbool.h>
#include "assembler.h"

/*===============================================
*   FUNCTION    :   assemble
*   DESCRIPTION :   This function assembles the script.asm file into formatted TRACS "C" code.
*   ARGUMENTS   :   bool *success, char **lines, const char *filename
*   RETURNS     :   int
 *==============================================*/
int assemble()
{
    //-------------- ENTRY POINT OF THE PROGRAM --------------//
    // Initialization of variables
    int line_count;
    unsigned int address = 0x000;
    unsigned int temp_address = 0x000;
    bool hasEOP = false;
    int label_count = 0;
    LABEL labels[MAX_LINES];

    // Step 1: Read the assembly code from the array and store it in an array of LINE structs
    LINE *lines = process_file("script.asm", &line_count);
    if (lines == NULL) // Error Handling Case 1: No Lines Found
    {
        printf("Error reading lines\n");
        return 1;
    }

    // // Printing all lines
    // for (int i = 0; i < line_count; i++)
    // {
    //     printf("Label: %s, Operation: %s, Operand: %s\n", lines[i].label, lines[i].operation, lines[i].operand);
    // }

    // Step 2: If set, load address, else set to 0x000
    set_address(&address, line_count, lines);
    temp_address = address; // Saving address to temp_address for later use (Labels)

    // Step 3: Create a label array and populate it with labels and their addresses
    for (int i = 1; i < line_count; i++)
    {
        if(strcmp(lines[i].label, "EOP") == 0 || strcmp(lines[i].operation, "EOP") == 0) 
            hasEOP = true;
        if (lines[i].label[0] != '\0')
        {
            strcpy(labels[label_count].label, lines[i].label);
            labels[label_count].address = temp_address;
            label_count++;
        }
        temp_address += 2; // Increment the address for each line
    }
    if(!hasEOP) 
    {
        printf("Error: No EOP found\n");
        return 1;
    }
    // Step 4: We need to iterate through all operands, then check if there is a label as an operand.
    //          if there is a label, we will check if it is in the labels array, 
    //          if a label marked as an operand is not found in the labels array, 
    //          we will print an error message.
    bool hasInvalidLabel = false;
    for (int i = 1; i < line_count; i++)
    {
        if (lines[i].operand[0] != '\0' && strncmp(lines[i].operand, "0x", 2) != 0)
        {
            int labelFound = 0;
            for (int j = 0; j < label_count; j++)
            {
                if (strcmp(lines[i].operand, labels[j].label) == 0)
                {
                    labelFound = 1;
                    break;
                }
            }
            if (!labelFound)
            {
                hasInvalidLabel = true;
                printf("Error: Unknown Label: %s\n", lines[i].operand);
            }
        }
    }


    //-------------- Writing Output to Text Document --------------//
    FILE *output_file = fopen("translation.txt", "w"); // Open file for writing

    //-------------- Writing Output to Text Document --------------//
    char filename[50]; // Allocate space for the filename
    sprintf(filename, "translation.txt"); // Generate filename with timestamp
    if (output_file == NULL) {
        printf("Error opening output file\n");
        return 1; // Return error code if file cannot be opened
    }

    // Redirect standard output to the file
    dup2(fileno(output_file), STDOUT_FILENO);

    // Print formatted TRACS code
    for (int i = 1; i < line_count; i++)
    {
        char *opcode = get_opcode(lines[i].operation);
        if (opcode == NULL)
        {
            printf("Invalid instruction: %s\n", lines[i].operation);
            continue;
        }
        // printf("ADDR = 0x%x; BUS = %s; MainMemory(); // %s\n", address, opcode, lines[i].operation);
        printf("0x%x %s\t", address, opcode);
        // Need to check if the operation is in labels, if it is, then we will print
        // the label's address
        int labelFound = 0;
        for (int j = 0; j < label_count; j++)
        {
            if (strcmp(lines[i].operand, labels[j].label) == 0)
            {
                // printf("ADDR = 0x%x; BUS = 0x%x; MainMemory();\n", address + 1, labels[j].address);
                printf("0x%x 0x%x\n", address + 1, labels[j].address);
                labelFound = 1;
                break;
            }
        }

        // Check if the operand is empty, treat it as 0x00
        if (!labelFound && lines[i].operand[0] == '\0')
        {
            // printf("ADDR = 0x%x; BUS = 0x00; MainMemory();\n", address + 1);
            printf("0x%x 0x00\n", address + 1);
            labelFound = 1;
        }

        // Check if the operand is in the format of an address string (e.g., 0x053)
        if (!labelFound && strncmp(lines[i].operand, "0x", 2) == 0)
        {
            // Check if the remaining characters are digits
            int k = 2;
            while (lines[i].operand[k] != '\0')
            {
                if (!isdigit(lines[i].operand[k]))
                {
                    // printf("Warning: Unknown Label: %s Writing opcode %s\n", lines[i].operand, lines[i].operation);
                    break;
                }
                k++;
            }

            // If all characters after "0x" are digits, treat it as an address string
            if (lines[i].operand[k] == '\0')
            {
                // printf("ADDR = 0x%x; BUS = %s; MainMemory();\n", address + 1, lines[i].operand);
                printf("0x%x %s\n", address + 1, lines[i].operand);
                labelFound = 1;
            }
        }

        // If the operand is neither a label nor an address string, print warning
        if (!labelFound)
        {
            // printf("Warning: Unknown Label: %s Writing opcode %s\n", lines[i].operand, lines[i].operation);
        }
        address += 2;
    }

    // Close the output file
    fclose(output_file);

    //-------------- EXIT POINT OF THE PROGRAM --------------//
    free(lines);
    return 0;
}


/*===============================================
*   FUNCTION    :   set_address
*   DESCRIPTION :   This function will set the address of the instruction.
*   ARGUMENTS   :   unsigned int *address
*   RETURNS     :   VOID
 *==============================================*/
void set_address(unsigned int *address, int line_count, LINE *lines)
{
    bool orgFound = false;
    for (int i = 0; i < line_count; i++)
    {
        if (strcmp(lines[i].label, "ORG") == 0)
        {
            orgFound = true;
            *address = strtol(lines[i].operation, NULL, 0);
            return;
        }
    }
    if(orgFound == false)
        *address = 0x000;
}

/*===============================================
*   FUNCTION    :   process_file
*   DESCRIPTION :   This function will read the assembly code from the file and store it in an array of LINE structs.
*   ARGUMENTS   :   const char *filename, int *line_count
*   RETURNS     :   LINE *
 *==============================================*/
LINE* process_file(const char *filename, int *line_count) {
    // Open the file
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file %s\n", filename);
        return NULL;
    }

    // Allocate memory for the array of lines
    LINE *lines = malloc(MAX_LINES * sizeof(LINE));
    if (lines == NULL) {
        printf("Memory allocation failed\n");
        fclose(fp);
        return NULL;
    }

    // Read the file LINE by LINE
    char LINE[MAX_LINE_LENGTH];
    *line_count = 0;
    while (fgets(LINE, sizeof(LINE), fp))
    {
        // Remove comments
        char *comment = strchr(LINE, ';');
        if (comment != NULL)
            *comment = '\0'; // Terminate the string at the comment position

        // Remove leading and trailing whitespace
        char *start = LINE;
        while (*start == ' ' || *start == '\t') start++;
        char *end = start + strlen(start) - 1;
        while (end > start && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) end--;
        *(end + 1) = '\0';

        // Parse the LINE and store it in the array
        if (strlen(start) > 0)
        {
            sscanf(start, "%s %s %s", lines[*line_count].label, lines[*line_count].operation, lines[*line_count].operand);
            (*line_count)++;
        }
    }

    // Close the file
    fclose(fp);

    // Format Correction
    for (int i = 0; i < *line_count; i++)
    {
        if (lines[i].label[0] != '\0' && (
                strcmp(lines[i].label, "WM") == 0 || strcmp(lines[i].label, "RM") == 0 ||
                strcmp(lines[i].label, "WACC") == 0 || strcmp(lines[i].label, "WIB") == 0 ||
                strcmp(lines[i].label, "WIO") == 0 || strcmp(lines[i].label, "RACC") == 0 ||
                strcmp(lines[i].label, "ADD") == 0 || strcmp(lines[i].label, "SUB") == 0 ||
                strcmp(lines[i].label, "MUL") == 0 || strcmp(lines[i].label, "AND") == 0 ||
                strcmp(lines[i].label, "OR") == 0 || strcmp(lines[i].label, "NOT") == 0 ||
                strcmp(lines[i].label, "XOR") == 0 || strcmp(lines[i].label, "SHL") == 0 ||
                strcmp(lines[i].label, "SHR") == 0 || strcmp(lines[i].label, "BR") == 0 ||
                strcmp(lines[i].label, "BRE") == 0 || strcmp(lines[i].label, "BRNE") == 0 ||
                strcmp(lines[i].label, "BRGT") == 0 || strcmp(lines[i].label, "BRLT") == 0 ||
                strcmp(lines[i].label, "EOP") == 0 || strcmp(lines[i].label, "SWAP") == 0 ||
                strcmp(lines[i].label, "WB") == 0
            ))
        {
            // This means it's not a label, so move everything to the right
            strcpy(lines[i].operand, lines[i].operation);
            strcpy(lines[i].operation, lines[i].label);
            strcpy(lines[i].label, "");
        }
    }
    return lines;
}

/*===============================================
*   FUNCTION    :   get_opcode
*   DESCRIPTION :   This function will return the opcode based on the instruction.
*   ARGUMENTS   :   char *instruction
*   RETURNS     :   char *
 *==============================================*/
char *get_opcode(char *instruction) {
    if (strcmp(instruction, "WB") == 0)             return "0x30";
    else if (strcmp(instruction, "WM") == 0)        return "0x0C";
    else if (strcmp(instruction, "RM") == 0)        return "0x14";
    else if (strcmp(instruction, "WACC") == 0)      return "0x58";
    else if (strcmp(instruction, "WIB") == 0)       return "0x38";
    else if (strcmp(instruction, "WIO") == 0)       return "0x28";
    else if (strcmp(instruction, "RACC") == 0)      return "0x58";
    else if (strcmp(instruction, "ADD") == 0)       return "0xF0";
    else if (strcmp(instruction, "SUB") == 0)       return "0xE8";
    else if (strcmp(instruction, "MUL") == 0)       return "0xD8";
    else if (strcmp(instruction, "AND") == 0)       return "0xD0";
    else if (strcmp(instruction, "OR") == 0)        return "0xC8";
    else if (strcmp(instruction, "NOT") == 0)       return "0xC0";
    else if (strcmp(instruction, "XOR") == 0)       return "0xB8";
    else if (strcmp(instruction, "SHL") == 0)       return "0xB0";
    else if (strcmp(instruction, "SHR") == 0)       return "0xA8";
    else if (strcmp(instruction, "BR") == 0)        return "0x18";
    else if (strcmp(instruction, "BRE") == 0)       return "0xA0";
    else if (strcmp(instruction, "BRNE") == 0)      return "0x88";
    else if (strcmp(instruction, "BRGT") == 0)      return "0x90";
    else if (strcmp(instruction, "BRLT") == 0)      return "0x88";
    else if (strcmp(instruction, "EOP") == 0)       return "0xF8";
    else if (strcmp(instruction, "SWAP") == 0)      return "0x70";
    else return NULL;
}

/*===============================================
*   FUNCTION    :   printLabels
*   DESCRIPTION :   This function will print the labels and their addresses.
*   ARGUMENTS   :   int label_count, LABEL labels
*   RETURNS     :   VOID
 *==============================================*/
void printLabels(int label_count, LABEL *labels)
{
    for (int i = 0; i < label_count; i++)
    {
        printf("Label: %s, Address: %x\n", labels[i].label, labels[i].address);
    }
}

