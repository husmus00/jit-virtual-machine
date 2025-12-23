#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "config.h"
#include "vm.h"
#include "types.h"

void interpret_bytecode(vm_state* state);

void op_add(vm_state* state);
void op_sub(vm_state* state);
void op_psh(vm_state* state);
void op_pop(vm_state* state);
void op_dup(vm_state* state);
void op_prt(vm_state* state);
void op_str(vm_state* state);
void op_lod(vm_state* state);
void op_jmp(vm_state* state);
void op_cal(vm_state* state); 
void op_ret(vm_state* state);


#endif // INTERPRETER_H
