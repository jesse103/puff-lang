#include "include/asm.h"
#include "include/attributes.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char* declarations;
char* main_function;
bool selecting_main_function = false;
list_t* attributes_appending = 0;

char* as_f_compound(ast_t* ast) 
{
    char* value = calloc(1, sizeof(char));
    /*for(int i = 0; i < (int)ast->children->size; i++)
    {
        ast_t* child_ast = (ast_t*)ast->children->items[i];
        char* next_value = as_f(child_ast);
        if(next_value)
        {
            value = realloc(value, (strlen(next_value) + 1) * sizeof(char));
            strcat(value, next_value);
        }
    }*/
    for (unsigned int i = 0; i < ast->children->size; i++)
    {
        ast_t* child_ast = (ast_t*) ast->children->items[i];
        char* next_value = as_f(child_ast);
        value = realloc(value, (strlen(value) + strlen(next_value) + 1) * sizeof(char));
        strcat(value, next_value);
    }
    return value;
}
char* as_f_assignment(ast_t* ast) 
{
    if(!declarations)
    {
        char* begin = "\n.data\n";
        declarations = calloc(1, (strlen(begin) + 1) * sizeof(char));
        sprintf(declarations, begin, begin);
    }
    char* s = calloc(1, sizeof(char));
    if(attributes_appending)
    {
        for(unsigned int i = 0; i < attributes_appending->size; i++)
        {
            ast_t* child = (ast_t*)attributes_appending->items[i];
            appendAttribute(ast, child);
        }
        attributes_appending = (void*)0;
    }
    if(ast->value->type == AST_FUNCTION)
    {
        if(ast->isMainFunction == true)
        {
            main_function = calloc(1, (strlen(ast->name) + 1) * sizeof(char));
            strcat(main_function, ast->name);
            selecting_main_function = false;
            ast->name = "__main";
            printf("[ASM]: Found main function: `%s`.\n", main_function);
        }
        const char* template = "\n.global %s\n"
                                "%s:\n";
        s = realloc(s, (strlen(template) + (strlen(ast->name)*2)+1) * sizeof(char));
        sprintf(s, template, ast->name, ast->name);
        ast_t* as_val = ast->value;
        char* as_val_val = as_f(as_val->value);
        s = realloc(s, (strlen(s) + strlen(as_val_val) + 1) * sizeof(char));
        strcat(s, as_val_val);
    }
    else if(ast->value->type == AST_STRING)
    {
        const char* template = "\n%s:\n"
                               ".ascii \"%s\"\n";
        char* ret_s = calloc(strlen(template) + 128, sizeof(char));
        sprintf(ret_s, template, ast->value->name, ast->value->string_value);
        declarations = realloc(declarations, (strlen(declarations) + strlen(ret_s) + 1) * sizeof(char));
        strcat(declarations, ret_s);
        free(ret_s);
    }
    return s;
}
char* as_f_variable(ast_t* ast) 
{
    
}
char* as_f_call(ast_t* ast) 
{
    // TODO: make less messy
    char* s = calloc(1, sizeof(char));
    if(strcmp(ast->name, "return") == 0)
    {
        ast_t* first_arg = (ast_t*)ast->value->children->size ? ast->value->children->items[0] : (void*) 0;
        const char* template = "\tmov $%d, %%eax\n"
                                "\tret\n";
        char* ret_s = calloc(strlen(template) + 128, sizeof(char));
        sprintf(ret_s, template, first_arg ? first_arg->int_value : 0);
        s = realloc(s, (strlen(ret_s)+1) * sizeof(char));
        strcat(s, ret_s);
        free(ret_s);
    }else if(strcmp(ast->name, "call") == 0)
    {
        ast_t* first_arg = (ast_t*)ast->value->children->size ? ast->value->children->items[0] : (void*) 0;
        const char* template = "jmp %s\n";
        char* ret_s = calloc(strlen(template) + 128, sizeof(char));
        sprintf(ret_s, template, first_arg->string_value);
        s = realloc(s, (strlen(s) + strlen(ret_s) + 1) * sizeof(char));
        strcat(s, ret_s);
        free(ret_s);
    }else if(strcmp(ast->name, "asm") == 0)
    {
        for(unsigned int i = 0; i < ast->value->children->size; i++)
        {
            ast_t* child = (ast_t*)ast->value->children->items[i];
            char* ret_s = calloc(strlen(child->string_value) + 128, sizeof(char));
            sprintf(ret_s, child->string_value, child->string_value);
            strcat(ret_s, "\n");
            s = realloc(s, (strlen(s) + strlen(ret_s) + 1) * sizeof(char));
            strcat(s, ret_s);
            free(ret_s);
        }
    }else if(strcmp(ast->name, "print") == 0) {
        ast_t* first_arg = (ast_t*)ast->value->children->size ? ast->value->children->items[0] : (void*)0;
        const char* template = "\tmov $1, \%rax\n"
                               "\tmov $1, \%rdi\n"
                               "\tmov $%s, \%rsi\n"
                               "\tmov $13, \%rdx\n"
                               "\tsyscall\n";
        char* ret_s = calloc(strlen(template) + 128, sizeof(char));
        sprintf(ret_s, template, first_arg->string_value, first_arg->string_value, first_arg->string_value);
        s = realloc(s, (strlen(s) + strlen(ret_s) + 1) * sizeof(char));
        strcat(s, ret_s);
        free(ret_s);
    } else {
        const char* template = "call %s\n";
        char* ret_s = calloc(strlen(template) + 128, sizeof(char));
        sprintf(ret_s, template, ast->name);
        s = realloc(s, (strlen(s) + strlen(ret_s) + 1) * sizeof(char));
        strcat(s, ret_s);
        free(ret_s);
    }
    return s;
}

char* as_f_int(ast_t* ast)
{
}

char* as_f_string(ast_t* ast)
{
    
}

char* as_f_attribute(ast_t* ast)
{
    attributes_appending = ast->children;
    //for(int i = 0; i < ast->children->size; i++)
        //printf("name: %s\n", ((ast_t*)ast->children->items[i])->string_value);
    return "";
}

char* as_f_root(ast_t* ast)
{
    const char* section_text = ".global _start\n\n"
                               ".text\n\n"
                               "_start:\n"
                               "\tcall __main\n"
                               "\tmov \%eax, \%ebx\n"
                               "\tmov $1, \%eax\n"
                               "\tint $0x80\n\n";
    char* value = (char*)calloc((strlen(section_text) + 128), sizeof(char));
    strcpy(value, section_text);
    char* next_value = as_f(ast);
    value = (char*)realloc(value, (strlen(value) + (strlen(next_value) + 1)) * sizeof(char));
    strcat(value, next_value);
    return value;
}

char* as_f(ast_t* ast)
{
    char* value = calloc(1, sizeof(char));
    char* next_value = 0;
    switch (ast->type)
    {
        case AST_COMPUND: next_value = as_f_compound(ast); break;
        case AST_ASSIGNMENT: next_value = as_f_assignment(ast); break;
        case AST_VARIABLE: next_value = as_f_variable(ast); break;
        case AST_CALL: next_value = as_f_call(ast); break;
        case AST_INT: next_value = as_f_int(ast); break;
        case AST_STRING: next_value = as_f_string(ast); break;
        case AST_ATTRIBUTE: next_value = as_f_attribute(ast); break;
        default: { printf("[ASM Frontend]: No frontend for AST of type `%d`.\n", ast->type); exit(1); } break;
    }
    value = realloc(value, (strlen(next_value) + 1) * sizeof(char));
    strcat(value, next_value);
    return value;
}

char* make_declarations()
{
    return declarations;
}