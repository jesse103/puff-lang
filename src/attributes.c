#include "include/attributes.h"
#include <stdlib.h>

list_t* loaded_attributes;

void setMainFunction(ast_t* ast)
{
    ast->isMainFunction = true;
}

void appendAttribute(ast_t* ast, ast_t* attribute)
{
    char* string_value = attribute->string_value;
    if(!string_value) 
        return;
    for(unsigned int i = 0; i < loaded_attributes->size; i++)
    {
        attribute_t* attr = (attribute_t*)loaded_attributes->items[i];
        if(!attr) 
            continue;
        if(strcmp(attr->name, string_value) == 0)
        {
            attr->function(ast);
            break;
        }
    }
}

attribute_t* init_attribute(char* name, attribute_func function)
{
    attribute_t* attr = calloc(1, sizeof(struct ATTRIBUTE_STRUCT));
    attr->name = name;
    attr->function = function;
    list_push(loaded_attributes, attr);
    return attr;
}

void load_attributes()
{
    loaded_attributes = init_list(sizeof(struct ATTRIBUTE_STRUCT*));
    init_attribute("main_func", setMainFunction);
}