#include "include/attributes.h"

void setMainFunction(ast_t* ast)
{
    ast->isMainFunction = true;
}

void appendAttribute(ast_t* ast, ast_t* attribute)
{
    char* string_value = attribute->string_value;
    if(strcmp(string_value, "main_func") == 0)
        setMainFunction(ast);
}