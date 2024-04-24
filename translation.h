#ifndef TRANSLATION_H
#define TRANSLATION_H
/*===============================================
 *   STRUCTS & DEFINITIONS
 *==============================================*/

#define MAX_LINES 1000

typedef struct machine_code_line {
    char opcode[8];
    char operand[8];
    char opcodeAddress[8];
    char operandAddress[8];
} MACHINE_CODE_LINE;

/*===============================================
 *   FUNCTION PROTOTYPES
 *==============================================*/
MACHINE_CODE_LINE* interpretTranslation(const char* filename, int* line_count);

#endif
