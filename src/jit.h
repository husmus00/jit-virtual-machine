#ifndef JIT_H
#define JIT_H

#include "config.h"
#include "interpreter.h"

#include <stddef.h> 


#define NATIVE_ADD 0 // Testing
#define NATIVE_INC 1 // Testing

typedef struct jitc jitc;

struct jitc{
    int* bytecode;
    int bytecode_index;
    unsigned char native_code[PROGRAM_SIZE];
    size_t native_size;
};

// Step 1: Define the function pointer type
typedef void (*emit_function)(jitc* jc);

typedef int* (*NativeFunc)(int*, void*, void*); // Takes the stack pointer, global/local memory pointer, and helper function array


void native_test(vm_state* state);

#endif // JIT_H