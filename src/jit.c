#include "jit.h"

#include "config.h"
#include "opcodes.h"
#include "interpreter.h"
#include "emit.h"


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

// Testing

void write_native_code(void** native_address, unsigned char* machine_code,  size_t code_size) {
    // Step 1: Allocate writable memory (page-aligned)
    size_t page_size = getpagesize(); // Get system page size (e.g., 4096)
    void* new_native_memory = mmap(
        NULL,
        page_size,
        PROT_READ | PROT_WRITE, // Writable initially
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1,
        0
    );
    if (new_native_memory == MAP_FAILED) {
        perror("mmap failed\n");
        // return 1;
    }
 
    // Step 2: Write raw machine code into the memory
    
    if (code_size > page_size) {
        fprintf(stderr, "Machine code too large for page\n");
        munmap(new_native_memory, page_size);
        // return 1;
    }
    __builtin_memcpy(new_native_memory, machine_code, code_size);

#ifdef __aarch64__
    // Important: On ARM, instruction cache != data cache
    // Need to flush the d-cache before executing
    __builtin___clear_cache(new_native_memory, 
                           (char*)new_native_memory + sizeof(machine_code));
#endif
 
    // Step 3: Make the memory executable (remove write access)
    if (mprotect(new_native_memory, page_size, PROT_READ | PROT_EXEC) == -1) {
        perror("mprotect failed\n");
        munmap(new_native_memory, page_size);
        // return 1;
    }
 
    // Step 4: Cast to function pointer and call

    *native_address = new_native_memory;
 
    // Cleanup
    //munmap(native_memory, page_size);
}

void jit_compile(vm_state* state) {
    unsigned char native_code[PROGRAM_SIZE];

    size_t code_size = 0;
    size_t snippet_size = 0;

    emit_add(native_code + code_size, &snippet_size);
    code_size += snippet_size;

    emit_ret(native_code + code_size, &snippet_size);
    code_size += snippet_size;

    if (DEBUG) {
        printf("Contents of native_code after emission:");
        for (int i = 0; i < code_size; i++) {
            printf("%x, ", native_code[i]);
        }
        printf("\n");
    }

    // void* native_func_address = state->native_funcs[NATIVE_ADD];

    //printf("Native address before: %p\n", state->native_funcs[NATIVE_ADD]);

    write_native_code(&state->native_funcs[NATIVE_ADD], native_code, code_size);

    //printf("Native address after: %p\n", state->native_funcs[NATIVE_ADD]);
}


void native_test(vm_state* state) {

    jit_compile(state);

    NativeFunc native_add = (NativeFunc)state->native_funcs[NATIVE_ADD];

    int* result = native_add(state->stack + state->sp);

    state->sp = result - state->stack;

    if (DEBUG) {
        printf("Return from native add is: %p\n", result);
    }
}



 