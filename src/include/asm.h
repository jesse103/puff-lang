#ifndef PUFF_ASM_H
#define PUFF_ASM_H

#include "ast.h"
#include "types.h"

char* as_f(ast_t* ast);
char* as_f_root(ast_t* ast);
char* make_declarations();

#endif