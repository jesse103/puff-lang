#ifndef PUFF_AST_H
#define PUFF_AST_H

#include "list.h"
#include "types.h"
#include "stack_frame.h"

typedef struct AST_STRUCT
{
    enum
    {
        AST_FUNCTION,
        AST_CALL,
        AST_COMPUND,
        AST_VARIABLE,
        AST_STATEMENT,
        AST_NOOP,
        AST_DEFINITION_TYPE,
        AST_ASSIGNMENT,
        AST_INT,
        AST_ACCESS,
        AST_ATTRIBUTE,
        AST_STRING
    } type;

    list_t* children;
    char* name;
    struct AST_STRUCT* value;
    int data_type;
    int int_value;
    char* string_value;
    bool isMainFunction;
    stack_frame_t* stack_frame;
} ast_t;

ast_t* init_ast(int type);

#endif