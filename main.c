#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "assembler.h"

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
    return 1;
}
