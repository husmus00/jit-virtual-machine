#ifndef VM_H
#define VM_H

#include "config.h"
#include "types.h"

typedef struct frame frame;

struct frame{
    int local_mem[MEM_SIZE];                  // Local per function memory
    int return_address;
    char* label;                              // Points to label in jump table
};

typedef struct vm_state vm_state;

struct vm_state {
    int bytecode[PROGRAM_SIZE];         // Read-only program memory
    label jump_table[FUNC_TABLE_SIZE];  // Addresses to jump to
    int ip;                             // Instruction pointer
    int sp;                             // Stack pointer
    int stack[STACK_SIZE];              // Stack
    int global_mem[MEM_SIZE];           // Global memory
    frame* frame_stack[STACK_SIZE];     // Frame stack
    int fp;                             // Frame Pointer
    // Some object store?
    void* native_funcs[FUNC_TABLE_SIZE];  // Pointers to native functions
    void* helper_funcs[FUNC_TABLE_SIZE];  // Pointers to helper functions (to be called from ASM)
};

void print_jump_table(vm_state* state);
void print_bytecode(vm_state* state);
void print_stack(vm_state* state);
void print_global_memory(vm_state* state);
void print_vm_state(vm_state* state);


#endif // VM_H