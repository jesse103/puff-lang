#include "include/parser.h"
#include "include/types.h"
#include "include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


parser_t* init_parser(lexer_t* lexer)
{
  parser_t* parser = calloc(1, sizeof(struct PARSER_STRUCT));
  parser->lexer = lexer;
  parser->token = lexer_next_token(lexer);

  return parser;
}

token_t* parser_eat(parser_t* parser, int type)
{
  if (parser->token->type != type)
  {
    printf("[Parser]: Unexpected token: `%s`, was expecting: `%s`\n", token_to_str(parser->token), token_type_to_str(type));
    exit(1);
  }

  parser->token = lexer_next_token(parser->lexer);
  return parser->token;
}

ast_t* parser_parse(parser_t* parser)
{
  return parser_parse_compound(parser);
}

bool isalpha_str(char* str)
{
  int len = strlen(str);
  int i;
  bool alpha = true;
  while(i < len)
  {
    if(!isalpha(str[i++]))
      alpha = false;
  }
  return alpha;
}

bool isdigit_str(char* str)
{
  int len = strlen(str);
  int i;
  bool digit = true;
  while(i < len)
  {
    if(!isdigit(str[i++]))
      digit = false;
  }
  return digit;
}

ast_t* parser_parse_id(parser_t* parser)
{
  //printf("[Parser]: Found ID `%s`\n", parser->token->value);
  char* value = calloc(strlen(parser->token->value) + 1, sizeof(char));
  strcpy(value, parser->token->value);
  parser_eat(parser, TOKEN_ID); 

  ast_t* ast = init_ast(AST_VARIABLE);
  ast->name = value;


  if (parser->token->type == TOKEN_COLON)
  {
    parser_eat(parser, TOKEN_COLON);

    while (parser->token->type == TOKEN_ID)
    {
      ast->data_type = typename_to_int(parser->token->value);
      parser_eat(parser, TOKEN_ID);

      if (parser->token->type == TOKEN_LT)
      {
        parser_eat(parser, TOKEN_LT);
        ast->data_type += typename_to_int(parser->token->value);
        parser_eat(parser, TOKEN_ID);
        parser_eat(parser, TOKEN_GT);
      }
    }
  }
  else
  {
    if (parser->token->type == TOKEN_LPAREN)
    {
      ast->type = AST_CALL;
      ast->value = parser_parse_list(parser);
    }
    else
    if (parser->token->type == TOKEN_LBRACKET)
    {
      parser_eat(parser, TOKEN_LBRACKET);
      ast->type = AST_ACCESS;
      if(isdigit_str(parser->token->value))
        ast->int_value = atoi(parser->token->value);
      else if(isalpha_str(parser->token->value))
        ast->string_value = parser->token->value;

      parser_eat(parser, TOKEN_INT);
      parser_eat(parser, TOKEN_RBRACKET);
    }
  }

  if (parser->token->type == TOKEN_EQUALS)
  {
    parser_eat(parser, TOKEN_EQUALS);
    ast->type = AST_ASSIGNMENT;
    ast->name = value;

    ast->value = parser_parse_expr(parser);
    
    if (ast->value->type != AST_CALL)
      ast->value->name = mkstr(ast->name);
  }

  return ast;
}

ast_t* parser_parse_block(parser_t* parser)
{
  parser_eat(parser, TOKEN_LBRACE);
  
  ast_t* ast = init_ast(AST_COMPUND);

  while (parser->token->type != TOKEN_RBRACE)
  {
    list_push(ast->children, parser_parse_expr(parser));
  }

  parser_eat(parser, TOKEN_RBRACE);

  return ast;
}

ast_t* parser_parse_list(parser_t* parser)
{
  unsigned int is_bracket = parser->token->type == TOKEN_ATTRIBUTE_BEGIN;

  parser_eat(parser, is_bracket ? TOKEN_ATTRIBUTE_BEGIN : TOKEN_LPAREN);

  ast_t* ast = init_ast(is_bracket ? AST_ATTRIBUTE : AST_COMPUND);
  
  if (parser->token->type != (is_bracket ? TOKEN_ATTRIBUTE_END : TOKEN_RPAREN))
  {
    list_push(ast->children, parser_parse_expr(parser));

    while (parser->token->type == TOKEN_COMMA)
    {
      parser_eat(parser, TOKEN_COMMA);
      list_push(ast->children, parser_parse_expr(parser));
    }
  }
 
  parser_eat(parser, is_bracket ? TOKEN_ATTRIBUTE_END : TOKEN_RPAREN);

  if (parser->token->type == TOKEN_COLON)
  {
    parser_eat(parser, TOKEN_COLON);

    while (parser->token->type == TOKEN_ID)
    {
      ast->data_type = typename_to_int(parser->token->value);
      parser_eat(parser, TOKEN_ID);

      if (parser->token->type == TOKEN_LT)
      {
        parser_eat(parser, TOKEN_LT);
        ast->data_type += typename_to_int(parser->token->value);
        parser_eat(parser, TOKEN_ID);
        parser_eat(parser, TOKEN_GT);
      }
    }
  }

  if (parser->token->type == TOKEN_RIGHT_ARROW)
  {
    parser_eat(parser, TOKEN_RIGHT_ARROW);
    ast->type = AST_FUNCTION;
    ast->value = parser_parse_compound(parser);

    // TODO: implement this
    for (unsigned int i = 0; i < ast->children->size; i++)
      ((ast_t*)ast->children->items[i])->type = AST_ASSIGNMENT;
  }

  return ast;
}

ast_t* parser_parse_int(parser_t* parser)
{
  int int_value = atoi(parser->token->value);
  parser_eat(parser, TOKEN_INT);

  ast_t* ast = init_ast(AST_INT);
  ast->int_value = int_value;

  return ast;
}

ast_t* parser_parse_string(parser_t* parser)
{
  //printf("[Parser]: Parsing string `%s`.\n", parser->token->value);
  char* value = mkstr(parser->token->value);
  parser_eat(parser, TOKEN_STRING);
  ast_t* ast = init_ast(AST_STRING);
  ast->string_value = value;
}

ast_t* parser_parse_factor(parser_t* parser)
{
  switch (parser->token->type)
  {
    case TOKEN_ID: return parser_parse_id(parser);
    case TOKEN_LPAREN: return parser_parse_list(parser);
    case TOKEN_INT: return parser_parse_int(parser);
    case TOKEN_STRING: return parser_parse_string(parser);
    default: { printf("[Parser]: Unexpected token `%s`\n", token_to_str(parser->token)); exit(1); };
  } 
}

ast_t* parser_parse_term(parser_t* parser)
{
  ast_t* ast_left = parser_parse_factor(parser);

  

  return ast_left;
}

ast_t* parser_parse_expr(parser_t* parser)
{
  switch(parser->token->type)
    {
        case TOKEN_ID: return parser_parse_id(parser);
        case TOKEN_LPAREN: return parser_parse_list(parser);
        case TOKEN_INT: return parser_parse_int(parser);
        case TOKEN_STRING: return parser_parse_string(parser);
        case TOKEN_ATTRIBUTE_BEGIN: return parser_parse_list(parser);
        default: { printf("[Parser]: Unexpected token `%s`\n", token_to_str(parser->token)); exit(1); };
    }
}

ast_t* parser_parse_statement(parser_t* parser)
{
  
}

ast_t* parser_parse_attribute(parser_t* parser)
{
  return init_ast(AST_ATTRIBUTE);
}

ast_t* parser_parse_compound(parser_t* parser)
{
  unsigned int should_close = 0;

  if (parser->token->type == TOKEN_LBRACE)
  {
    parser_eat(parser, TOKEN_LBRACE);
    should_close = 1;
  }

  ast_t* compound = init_ast(AST_COMPUND);

    while(parser->token->type != TOKEN_EOF && parser->token->type != TOKEN_RBRACE)
    {
        ast_t* child = parser_parse_expr(parser);
        list_push(compound->children, child);
        if(parser->token->type == TOKEN_SEMI)
            parser_eat(parser, TOKEN_SEMI);
    }

  if (should_close)
    parser_eat(parser, TOKEN_RBRACE);

  return compound;
}