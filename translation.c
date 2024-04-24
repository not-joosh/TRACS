 /*======================================================================================================
* FILE        : translation.c
* AUTHOR      : Josh Ratificar (Hardware Lead)
*               Ben Cesar Cadungog (Software Lead)
*               Jeddah Laine Luce�ara  (Research Lead)
*               Harold Marvin Comendador (Documentation Lead)
* DESCRIPTION : This file contains the implementation of the functions that interpret the translation.txt file.
* COPYRIGHT   : 24 April, 2024
* REVISION HISTORY:
*   21 April, 2024: V1.0 - File Created, made interpreter function to read translation.txt file.
======================================================================================================*/
/*===============================================
 *   HEADER FILES
 *==============================================*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "translation.h"

MACHINE_CODE_LINE* interpretTranslation(const char* filename, int* line_count) 
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return NULL;
    }

    // Allocate memory for the array of MACHINE_CODE_LINE
    MACHINE_CODE_LINE* array = (MACHINE_CODE_LINE*)malloc(MAX_LINES * sizeof(MACHINE_CODE_LINE));
    if (array == NULL) {
        fclose(file);
        printf("Memory allocation failed.\n");
        return NULL;
    }

    *line_count = 0; // Initialize line count

    char line[100]; // Buffer to read each line
    while (fgets(line, sizeof(line), file) != NULL) {
        // Parsing the line
        sscanf(line, "%s %s %s %s",
               array[*line_count].opcodeAddress,
               array[*line_count].opcode,
               array[*line_count].operandAddress,
               array[*line_count].operand);
        (*line_count)++; // Increment line count
        if (*line_count >= MAX_LINES) {
            // Reached maximum number of lines, break out of loop
            printf("Maximum number of lines reached.\n");
            break;
        }
    }

    fclose(file);
    return array;
}