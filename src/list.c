#include "include/list.h"

list_t* init_list(size_t item_size)
{
    list_t* list = calloc(1, sizeof(struct LIST_STRUCT));
    list->size = 0;
    list->item_size = item_size;
    list->items = 0;
    return list;
}

void list_clear(list_t* list)
{
  for(int i = 0; i < list->size; i++)
    free(list->items[i]);
}

void list_push(list_t* list, void* item)
{
    list->size += 1;

  if (!list->items)
    list->items = calloc(1, list->item_size);
  else
    list->items = realloc(list->items, (list->size * list->item_size));

  list->items[list->size-1] = item;
}