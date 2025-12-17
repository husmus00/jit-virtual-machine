#include "jit.h"

#include "config.h"
#include "opcodes.h"
#include "interpreter.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

// Testing

void write_native_code(vm_state* state, unsigned char* machine_code,  size_t code_size) {
    // Step 1: Allocate writable memory (page-aligned)
    size_t page_size = getpagesize(); // Get system page size (e.g., 4096)
    void* native_memory = mmap(
        NULL,
        page_size,
        PROT_READ | PROT_WRITE, // Writable initially
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1,
        0
    );
    if (native_memory == MAP_FAILED) {
        perror("mmap failed");
        // return 1;
    }
 
    // Step 2: Write raw x86 machine code into the memory
    

    
    if (code_size > page_size) {
        fprintf(stderr, "Machine code too large for page\n");
        munmap(native_memory, page_size);
        // return 1;
    }
    __builtin_memcpy(native_memory, machine_code, code_size);

#ifdef __aarch64__
    // Important: On ARM, instruction cache != data cache
    // Need to flush before executing!
    __builtin___clear_cache(native_memory, 
                           (char*)native_memory + sizeof(machine_code));
#endif
 
    // Step 3: Make the memory executable (remove write access)
    if (mprotect(native_memory, page_size, PROT_READ | PROT_EXEC) == -1) {
        perror("mprotect failed");
        munmap(native_memory, page_size);
        // return 1;
    }
 
    // Step 4: Cast to function pointer and call

    state->native_code[NATIVE_ADD] = native_memory;
 
    // Cleanup
    //munmap(native_memory, page_size);
}

#ifdef __x86_64__

void emit_native_add(vm_state* state) {
    // Machine code for add_two (from Section 4):
    unsigned char machine_code[] = {
        0x8B, 0xC7,             // mov eax, edi  (first arg)
        0x03, 0xC6,             // add eax, esi  (second arg)
        0xC3                    // ret
    };
    size_t code_size = sizeof(machine_code);

    write_native_code(state, machine_code, code_size);
}

#elif defined(__aarch64__)

void emit_native_add(vm_state* state) {
    // AArch64 is little endian
    // unsigned char machine_code[] = {
    //     //0x1E, 0xE0, 0x21, 0x8B, // ADD X30, X0, X1
    //     0x00, 0x00, 0x01, 0x8B,   // ADD X0, X0, X1
    //     0xC0, 0x03, 0x5F, 0xD6,   // RET
    // };

    unsigned char machine_code[] = {
        0x01, 0xc4, 0x5f, 0xb8, 0x02, 0xc4, 0x5f, 0xb8, 0x23, 0x00, 0x02, 0x0b,
        0x03, 0x4c, 0x00, 0xb8, 0xc0, 0x03, 0x5f, 0xd6
    };
    size_t code_size = sizeof(machine_code);

    write_native_code(state, machine_code, code_size);
}

#endif

void native_test(vm_state* state) {

    NativeFunc native_add = (NativeFunc)state->native_code[NATIVE_ADD];
    //int result = native_add(20, 30);
    //state->stack[0] = 0;
    int* result = native_add(state->stack + state->sp);

    state->sp = result - state->stack;

    if (DEBUG) {
        printf("Return from native add is: %d\n", result);
    }
}



 