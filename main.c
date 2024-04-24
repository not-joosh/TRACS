 /*======================================================================================================
* FILE        : main.c
* AUTHOR      : Josh Ratificar (Hardware Lead)
*               Ben Cesar Cadungog (Software Lead)
*               Jeddah Laine Luce�ara  (Research Lead)
*               Harold Marvin Comendador (Documentation Lead)
* DESCRIPTION : This file contains the main function that assembles the assembly code and interprets the translation.txt file.
* COPYRIGHT   : 24 April, 2024
* REVISION HISTORY:
*   24 April, 2024: V1.0 - File Created, made main function to assemble and interpret translation.txt file. 
======================================================================================================*/
/*===============================================
 *   HEADER FILES
 *==============================================*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "assembler.h"
#include "translation.h"

/*===============================================
 *   FUNCTION PROTOTYPES
 *==============================================*/
int main()
{
    // Making a messageArray of strings
    if(assemble())
    {
        printf("Assembly successful!\n");
    }
    else
    {
        printf("Assembly failed!\n");
        return 0;
    }
    int line_count;
    MACHINE_CODE_LINE* array = interpretTranslation("translation.txt", &line_count);
    if (array == NULL) {
        printf("Failed to interpret translation.\n");
        return 1;
    }

    printf("Interpreted translation successfully.\n");

    // Printing the contents of the array
    for (int i = 0; i < line_count; i++) {
        printf("Line %d: Opcode Address: %s, Opcode: %s, Operand Address: %s, Operand: %s\n",
               i+1, array[i].opcodeAddress, array[i].opcode, array[i].operandAddress, array[i].operand);
    }

    // Free the allocated memory
    free(array);
    return 0;
}
