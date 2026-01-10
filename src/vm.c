#include "vm.h"

#include <stdio.h>

void print_jump_table(vm_state* state) {
    printf("\nAddress Table:\n");
    for (int i = 0; i < FUNC_TABLE_SIZE; i++) {
        if (state->jump_table[i].address != -1) {
            printf("Label %s at address %d\n", state->jump_table[i].name, state->jump_table[i].address);
        }
    }
    printf("\n");
}

void print_bytecode(vm_state* state) {
    printf("Bytecode: ");
    for (int i = 0; i < 30; i++) {
        printf("%02X ", (unsigned char)state->bytecode[i]);
    }
    printf("\n");
}

void print_stack(vm_state* state) {
    printf("Stack:    X ");
    for (int i = 0; i < 30; i++) {
        int entry = state->stack[i];
        printf("%d ", entry);
    }
    printf("\n%*s\n", 13+(state->sp)*2, "^");
}

void print_global_memory(vm_state* state) {
    printf("Global Memory: [ ");
    for (int i = 0; i < 30; i++) {
        int val = state->global_mem[i];
        printf("%d ", val);
    }
    printf("... ]\n");
}

void print_vm_state(vm_state* state) {
    print_jump_table(state);
    print_bytecode(state);
    print_stack(state);
    printf("Pointers: Instruction: %d, Stack %d\n", state->ip, state->sp);
}