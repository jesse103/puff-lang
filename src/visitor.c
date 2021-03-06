#include "include/visitor.h"
#include "include/utils.h"
#include <stdio.h>
#include <string.h>

static ast_t* var_lookup(list_t* list, const char* name)
{
  for(int i = 0; i < list->size; i++)
  {
      ast_t* child = (ast_t*)list->items[i];
      if(!child->name)
      {
          continue;
      }
      if(strcmp(child->name, name) == 0)
        return child;
  }

  return 0;
}

visitor_t* init_visitor()
{
  visitor_t* visitor = calloc(1, sizeof(struct VISITOR_STRUCT));
  visitor->object = init_ast(AST_COMPUND);

  return visitor;
}

ast_t* visitor_visit(visitor_t* visitor, ast_t* node, list_t* list, stack_frame_t* stack_frame)
{
  switch (node->type)
  {
    case AST_COMPUND: return visitor_visit_compound(visitor, node, list, stack_frame); break;
    case AST_ASSIGNMENT: return visitor_visit_assignment(visitor, node, list, stack_frame); break;
    case AST_VARIABLE: return visitor_visit_variable(visitor, node, list, stack_frame); break;
    case AST_CALL: return visitor_visit_call(visitor, node, list, stack_frame); break;
    case AST_INT: return visitor_visit_int(visitor, node, list, stack_frame); break;
    case AST_ACCESS: return visitor_visit_access(visitor, node, list, stack_frame); break;
    case AST_FUNCTION: return visitor_visit_function(visitor, node, list, stack_frame); break;
    case AST_STRING: return visitor_visit_string(visitor, node, list, stack_frame); break;
    case AST_ATTRIBUTE: return node;
    //case AST_BINOP: return visitor_visit_binop(visitor, node, list, stack_frame); break;
    //case ast_STATEMENT_RETURN: return visitor_visit_statement_return(visitor, node, list, stack_frame); break;
    default: { printf("[Visitor]: Don't know how to handle AST of type `%d`.\n", node->type); exit(1); } break;
  }

  return node;
}

ast_t* visitor_visit_compound(visitor_t* visitor, ast_t* node, list_t* list, stack_frame_t* stack_frame)
{
  ast_t* compound = init_ast(AST_COMPUND);
  compound->stack_frame = stack_frame;

  for (unsigned int i = 0; i < node->children->size; i++)
  {
    ast_t* x = visitor_visit(visitor, (ast_t*) node->children->items[i], list, stack_frame);
    list_push(compound->children, x);
  }

  return compound;
}

ast_t* visitor_visit_assignment(visitor_t* visitor, ast_t* node, list_t* list, stack_frame_t* stack_frame)
{
  list_push(list, node);
  ast_t* new_var = init_ast(AST_ASSIGNMENT);
  new_var->name = node->name;
  new_var->data_type = node->data_type;

  if (node->value)
    new_var->value = visitor_visit(visitor, node->value, list, stack_frame);
  
  //if (list->size == 0)
  //  list_push(list, new_var);

  //if (new_var->value)
  //if (new_var->value->type == ast_FUNCTION)
  //  list_push(visitor->object->children, new_var->value);
  

  //new_var->stack_index = stack_frame->stack->size;
  //new_var->stack_frame = stack_frame;
  list_push(stack_frame->stack, new_var->name);

  return new_var;
}

ast_t* visitor_visit_variable(visitor_t* visitor, ast_t* node, list_t* list, stack_frame_t* stack_frame)
{

  list_push(stack_frame->stack, 0);

  int index = 0;

  for (unsigned int i = 0; i < list->size; i++)
  {
    ast_t* child = (ast_t*) list->items[i];
    
    if (!child->name)
      continue;

    if (strcmp(child->name, node->name) == 0)
    {
      index = i + 1;
      break;
    }
  }

  //node->stack_index = index ? (index + 1) : list_indexof_str(stack_frame->stack, node->name);
  node->stack_frame = stack_frame;

  return node;
}

ast_t* visitor_visit_function(visitor_t* visitor, ast_t* node, list_t* list, stack_frame_t* stack_frame)
{
  ast_t* func = init_ast(AST_FUNCTION);
  func->name = node->name;
  func->data_type = node->data_type;
  func->children = init_list(sizeof(struct ast_STRUCT*));

  stack_frame_t* new_stack_frame = init_stack_frame();
  list_push(new_stack_frame->stack, 0); 
  list_push(new_stack_frame->stack, 0); 

  for (unsigned int i = 0; i < node->children->size; i++)
    list_push(func->children, (ast_t*) visitor_visit(visitor, (ast_t*) node->children->items[i], list, new_stack_frame));
  
  for (unsigned int i = 0; i < func->children->size; i++)
    list_push(list, func->children->items[i]);

  func->value = visitor_visit(visitor, node->value, list, new_stack_frame);
  func->stack_frame = new_stack_frame;

  return func;
}

ast_t* visitor_visit_call(visitor_t* visitor, ast_t* node, list_t* list, stack_frame_t* stack_frame)
{
  /*ast_t* n = init_ast(node->type);
  n->data_type = node->data_type;
  n->int_value = node->int_value;
  n->isMainFunction = node->isMainFunction;
  n->name = node->name;
  n->value = node->value;*/

  list_t* new_args = init_list(sizeof(struct AST_STRUCT*));

  for(unsigned int i = 0; i < node->value->children->size; i++)
  {
      ast_t* child = (ast_t*)node->value->children->items[i];
      if(child->name)
      {
          ast_t* var = var_lookup(list, child->name);
          if(var)
          {
            switch(var->value->type)
            {
            case AST_STRING: child->string_value = var->name; break;
            case AST_INT: { child->int_value = var->value->int_value;} break;
            }
          }
      }
  }

  node->stack_frame = stack_frame;

  return node;
}

ast_t* visitor_visit_int(visitor_t* visitor, ast_t* node, list_t* list, stack_frame_t* stack_frame)
{
  //node->stack_index = -((int)stack_frame->stack->size);
 // node->stack_frame = stack_frame;
  list_push(stack_frame->stack, mkstr("0"));
  return node;
}

ast_t* visitor_visit_string(visitor_t* visitor, ast_t* node, list_t* list, stack_frame_t* stack_frame)
{
  list_t* chunks = str_to_hex_chunks(node->string_value);
  
  list_push(stack_frame->stack, 0);
  //node->stack_index = -(stack_frame->stack->size + chunks->size);

  return node;
}

ast_t* visitor_visit_binop(visitor_t* visitor, ast_t* node, list_t* list, stack_frame_t* stack_frame)
{
  //ast_t* new_binop = init_ast(ast_BINOP);
  //new_binop->left = visitor_visit(visitor, node->left, list, stack_frame);
  //new_binop->op = node->op;
  //new_binop->right = visitor_visit(visitor, node->right, list, stack_frame);
  //return new_binop;
}

ast_t* visitor_visit_statement_return(visitor_t* visitor, ast_t* node, list_t* list, stack_frame_t* stack_frame)
{
  //ast_t* ast = init_ast(ast_STATEMENT_RETURN);
  //ast->value = visitor_visit(visitor, node->value, list, stack_frame);
  //return ast;
}

ast_t* visitor_visit_access(visitor_t* visitor, ast_t* node, list_t* list, stack_frame_t* stack_frame)
{
  list_push(stack_frame->stack, 0);
  //node->stack_index = (list_indexof_str(stack_frame->stack, node->name));

  return node;
}