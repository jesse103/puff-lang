#ifndef PUFF_ATTRIBUTES_H
#define PUFF_ATTRIBUTES_H

#include "ast.h"
#include "types.h"

typedef void(*attribute_func)(ast_t* ast);

typedef struct ATTRIBUTE_STRUCT
{
    char* name;
    attribute_func function;
} attribute_t;

void appendAttribute(ast_t* ast, ast_t* attribute);
attribute_t* init_attribute(char* name, attribute_func function);
void load_attributes();

#endif