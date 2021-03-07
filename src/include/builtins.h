#ifndef PUFF_BUILTIN_FUNCTIONS
#define PUFF_BUILTIN_FUNCTIONS

#include "types.h"
#include "ast.h"
#include "list.h"

typedef char*(*builtin_func)(ast_t*);

typedef struct BUILTIN_FUNCTION_STRUCT
{
    char* name;
    builtin_func func;
} builtin_t;

bool isBuiltIn(char* name);
builtin_t* getByName(char* name);
char* callBuiltIn(char* name, ast_t* ast);
void load_builtins();

#endif