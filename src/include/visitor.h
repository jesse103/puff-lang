#ifndef PUFF_VISITOR_H
#define PUFF_VISITOR_H
#include "ast.h"
#include "list.h"
#include "stack_frame.h"

typedef struct VISITOR_STRUCT
{
  ast_t* object;
} visitor_t;

visitor_t* init_visitor();

ast_t* visitor_visit(visitor_t* visitor, ast_t* node, list_t* list, stack_frame_t* stack_frame);

ast_t* visitor_visit_compound(visitor_t* visitor, ast_t* node, list_t* list, stack_frame_t* stack_frame);
ast_t* visitor_visit_assignment(visitor_t* visitor, ast_t* node, list_t* list, stack_frame_t* stack_frame);
ast_t* visitor_visit_variable(visitor_t* visitor, ast_t* node, list_t* list, stack_frame_t* stack_frame);
ast_t* visitor_visit_function(visitor_t* visitor, ast_t* node, list_t* list, stack_frame_t* stack_frame);
ast_t* visitor_visit_call(visitor_t* visitor, ast_t* node, list_t* list, stack_frame_t* stack_frame);
ast_t* visitor_visit_int(visitor_t* visitor, ast_t* node, list_t* list, stack_frame_t* stack_frame);
ast_t* visitor_visit_string(visitor_t* visitor, ast_t* node, list_t* list, stack_frame_t* stack_frame);
ast_t* visitor_visit_binop(visitor_t* visitor, ast_t* node, list_t* list, stack_frame_t* stack_frame);
ast_t* visitor_visit_statement_return(visitor_t* visitor, ast_t* node, list_t* list, stack_frame_t* stack_frame);
ast_t* visitor_visit_access(visitor_t* visitor, ast_t* node, list_t* list, stack_frame_t* stack_frame);
#endif