#include "include/stack_frame.h"

stack_frame_t* init_stack_frame()
{
  stack_frame_t* stack_frame = calloc(1, sizeof(struct STACK_FRAME_STRUCT));
  stack_frame->stack = init_list(sizeof(char*));

  return stack_frame;
}