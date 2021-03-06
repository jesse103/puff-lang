#ifndef TOKEN_H
#define TOKEN_H

typedef struct TOKEN_STRUCT
{
    char* value;
    enum
    {
        TOKEN_ID,
        TOKEN_EQUALS,
        TOKEN_LPAREN,
        TOKEN_RPAREN,
        TOKEN_COLON,
        TOKEN_COMMA,
        TOKEN_LT,
        TOKEN_GT,
        TOKEN_LBRACE,
        TOKEN_RBRACE,
        TOKEN_LBRACKET,
        TOKEN_RBRACKET,
        TOKEN_RIGHT_ARROW,
        TOKEN_SEMI,
        TOKEN_STATEMENT,
        TOKEN_STRING,
        TOKEN_INT,
        TOKEN_PLUS,
        TOKEN_MINUS,
        TOKEN_DIV,
        TOKEN_MUL,
        TOKEN_ATTRIBUTE_BEGIN,
        TOKEN_ATTRIBUTE_END,
        TOKEN_EOF
    } type;
} token_t;

token_t* init_token(char* value, int type);

const char* token_type_to_str(int type);

char* token_to_str(token_t* token);

#endif