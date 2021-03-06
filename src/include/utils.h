#ifndef PUFF_UTILS_H
#define PUFF_UTILS_H
#include "list.h"

void assert_not_null(void* x);
char* str_to_hex(const char* instr);
list_t* str_to_hex_chunks(const char* instr);
char* mkstr(const char* str);
char* str_format(char* instr);
char str_to_escape_seq(const char* instr);

#endif