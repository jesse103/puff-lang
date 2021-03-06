#ifndef PUFF_STACAK_FRAME
#define PUFF_STACAK_FRAME
#include "list.h"
typedef struct STACK_FRAME_STRUCT
{
  list_t* stack;
} stack_frame_t;

stack_frame_t* init_stack_frame();
#endif