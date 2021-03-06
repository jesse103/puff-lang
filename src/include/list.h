#ifndef PUFF_LIST_H
#define PUFF_LIST_H
#include <stdlib.h>
typedef struct LIST_STRUCT
{
    void** items;
    size_t size;
    size_t item_size;
} list_t;

void list_clear(list_t* list);

list_t* init_list(size_t item_size);

void list_push(list_t* list, void* item);

#endif