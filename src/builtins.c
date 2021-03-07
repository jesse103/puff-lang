#include "include/builtins.h"
#include <string.h>
#include "include/utils.h"

list_t* f_list;

void init_builtin(char* name, builtin_func function)
{
    builtin_t* func = calloc(1, sizeof(struct BUILTIN_FUNCTION_STRUCT));
    func->name = name;
    func->func = function;
    list_push(f_list, func);
    return func;
}

builtin_t* getByName(char* name)
{
    for(unsigned int i = 0; i < f_list->size; i++)
    {
        builtin_t* child = (builtin_t*)f_list->items[i];
        if(strcmp(child->name, name) == 0)
            return child;
    }
    return 0;
}

bool isBuiltIn(char* name)
{
    if(getByName(name))
    {
        return true;
    }
    return false;
}

char* callBuiltIn(char* name, ast_t* ast)
{
    builtin_t* func = getByName(name);
    if(func)
    {
        return func->func(ast);
    }
}

char* _return(ast_t* ast)
{
    ast_t* first_arg = ast->value->children->size ? (ast_t*)ast->value->children->items[0] : 0;
    char* return_buffer = calloc(1, sizeof(char));
    if(first_arg)
    {
        int value = first_arg->int_value ? first_arg->int_value : 0;
        const char* template = "\tmov $%d, %%eax\n"
                               "\tret";
        char* ret_s = calloc(strlen(template) + 128, sizeof(char));
        sprintf(ret_s, template, value);
        return_buffer = realloc(return_buffer, (strlen(return_buffer) + strlen(ret_s) + 1) * sizeof(char));
        strcat(return_buffer, ret_s);
    }
    return return_buffer;
}

char* _asm(ast_t* ast)
{
    char* return_buffer = calloc(1, sizeof(char));
    for(unsigned int i = 0; i < ast->value->children->size; i++)
    {
        ast_t* child = (ast_t*)ast->value->children->items[i];
        char* ret_s = calloc(strlen(child->string_value) + 128, sizeof(char));
        sprintf(ret_s, child->string_value, child->string_value);
        strcat(ret_s, "\n");
        return_buffer = realloc(return_buffer, (strlen(return_buffer) + strlen(ret_s) + 1) * sizeof(char));
        strcat(return_buffer, ret_s);
        free(ret_s);
    }
    return return_buffer;
}

int getTypeFromValues(ast_t* ast)
{
    if(ast->string_value) return AST_STRING;
    if(ast->int_value) return AST_INT;
    return 0;
}

char* print(ast_t* ast)
{
    ast_t* first_arg = ast->value->children->size ? (ast_t*)ast->value->children->items[0] : 0;
    char* return_buffer = calloc(1, sizeof(char));
    if(first_arg)
    {
        switch(first_arg->type)
        {
            case AST_VARIABLE: {
                const char* template = "\tmov $1, \%rax\n"
                                       "\tmov $1, \%rdi\n"
                                       "\tmov $%s, \%rsi\n"
                                       "\tmov $13, \%rdx\n"
                                       "\tsyscall\n";
                char* ret_s = calloc(strlen(template) + 128, sizeof(char));
                sprintf(ret_s, template, first_arg->string_value);
                return_buffer = realloc(return_buffer, (strlen(return_buffer) + strlen(ret_s) + 1) * sizeof(char));
                strcat(return_buffer, ret_s);
                break;
                }
        }
    }
    return return_buffer;
}

void load_builtins()
{
    f_list = init_list(sizeof(struct BUILTIN_FUNCTION_STRUCT*));
    init_builtin("print", print);
    init_builtin("return", _return);
    init_builtin("asm", _asm);
}