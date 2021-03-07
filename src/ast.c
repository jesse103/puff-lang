#include "include/ast.h"
#include <stdlib.h>

ast_t* init_ast(int type)
{
    ast_t* ast = calloc(1, sizeof(struct AST_STRUCT));
    ast->type = type;
    if(type == AST_COMPUND || type == AST_ATTRIBUTE)
        ast->children = init_list(sizeof(struct AST_STRUCT*));
    else if(type == AST_FUNCTION)
        ast->isMainFunction = false;
    ast->int_value = 0;
    ast->string_value = 0;
    return ast;
}