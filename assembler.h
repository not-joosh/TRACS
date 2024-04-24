#ifndef ASSEMBLER_H
#define ASSEMBLER_H
/*===============================================
 *   STRUCTS & DEFINITIONS
 *==============================================*/
#define MAX_LINE_LENGTH 1000
#define MAX_LINES 1000

typedef struct line {
    char label[MAX_LINE_LENGTH];
    char operation[MAX_LINE_LENGTH];
    char operand[MAX_LINE_LENGTH];
} LINE;

typedef struct label {
    char label[MAX_LINE_LENGTH];
    unsigned int address;
} LABEL;

/*===============================================
 *   FUNCTION PROTOTYPES
 *==============================================*/
LINE* process_file(const char *filename, int *line_count);
char* get_opcode(char *instruction);
void set_address(unsigned int *address, int line_count, LINE *lines);
void printLabels(int label_count, LABEL *labels);
int assemble();

#endif