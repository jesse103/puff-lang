#include "include/puff.h"
#include "include/lexer.h"
#include "include/io.h"
#include "include/parser.h"
#include "include/asm.h"
#include "include/visitor.h"
#include "include/attributes.h"
#include "include/builtins.h"
#include <stdlib.h>

void puff_compile(char* src)
{
    printf("[Puff]: Compiling..\n");

    load_attributes();
    load_builtins();

    lexer_t* lexer = init_lexer(src);
    parser_t* parser = init_parser(lexer);
    ast_t* root = parser_parse(parser);

    visitor_t* visitor = init_visitor();

    ast_t* optimized_root = visitor_visit(visitor, root, init_list(sizeof(struct AST_STRUCT*)), init_stack_frame());

    char* s = as_f_root(optimized_root);

    char* declarations = make_declarations(); // TODO: move to asm frontend
    s = realloc(s, (strlen(s) + strlen(declarations) + 1) * sizeof(char));
    strcat(s, declarations);

    //printf("%s\n", s);

    puff_write_file("a.s", s);
    system("as a.s -o compiled.o");
    system("ld compiled.o -o main.pf");
    system("if [ -f \"./main.pf\" ]; then \necho \"Compiled successfully.\n\"\nelse\necho \"Failed to compile\n\" \nfi"); // too lazy for file checking
}

void puff_compile_file(const char* filename)
{
    char* src = puff_read_file(filename);
    puff_compile(src);
    free(src);
}