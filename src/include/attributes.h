#ifndef PUFF_ATTRIBUTES_H
#define PUFF_ATTRIBUTES_H

#include "ast.h"
#include "types.h"

typedef struct ATTRIBUTE_STRUCT
{
    char* name;
    ast_function function;
} attribute_t;

void appendAttribute(ast_t* ast, ast_t* attribute);
attribute_t* init_attribute(char* name, ast_function);
void load_attributes();

#endif