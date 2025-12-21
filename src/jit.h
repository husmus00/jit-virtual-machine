#include "interpreter.h"

#define NATIVE_ADD 0 // Testing
#define NATIVE_INC 1 // Testing

///typedef int (*AddTwoFunc)(int, int);
typedef int* (*NativeFunc)(int*); // Takes the stack pointer (is that all it needs?)


void native_test(vm_state* state);