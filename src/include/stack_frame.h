#ifndef PUFF_STACK_FRAME
#define PUFF_STACK_FRAME
#include "list.h"
typedef struct STACK_FRAME_STRUCT
{
  list_t* stack;
} stack_frame_t;

// unused currently, will put back to use at a later date

stack_frame_t* init_stack_frame();
#endif