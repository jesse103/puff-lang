#include "include/puff.h"
#include "include/macros.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("no files given\n");
        return 1;
    }
    puff_compile_file(argv[1]);
    return 0;
}