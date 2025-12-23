#include "interpreter.h"
#include "jit.h"
#include "opcodes.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>

int get_jit_operand(jitc* jc) {
    return jc->bytecode[++(jc->bytecode_index)];
}

#define __aarch64__

#ifdef __aarch64__

void emit_psh(jitc* jc) {
    int val = get_jit_operand(jc);
    
    if (DEBUG) {
        printf("JIT PSH with operand %d\n", val);
    }

    // TODO: Add support for negative numbers and those larger than 16 bits
    // For now, only support PSHing unsigned vals up to 16 bits

    uint32_t movz_base = 0x52800001;  // movz w1, #0
    uint32_t movz = movz_base | ((uint32_t)val << 5);

    unsigned char byte_1 = movz & 0xFF;
    unsigned char byte_2 = (movz >> 8) & 0xFF;
    unsigned char byte_3 = (movz >> 16) & 0xFF;
    unsigned char byte_4 = (movz >> 24) & 0xFF;

    unsigned char psh[] = {
        byte_1, byte_2, byte_3, byte_4, // mov w1, #val
        0x01, 0x4c, 0x00, 0xb8,         // str w1, [x0, #4]!
    };

    memcpy(jc->native_code + jc->native_size, psh, sizeof(psh));
    jc->native_size += sizeof(psh);

    return;
}

void emit_add(jitc* jc) {
    if (DEBUG) {
        printf("JIT ADD\n");
    }

    unsigned char add[] = {
        0x01, 0xc4, 0x5f, 0xb8, // ldr w1, [x0], #-4 // Basically POP
        0x02, 0xc4, 0x5f, 0xb8, // ldr w2, [x0], #-4 // Basically POP
        0x23, 0x00, 0x02, 0x0b, // add w3, w1, w2 
        0x03, 0x4c, 0x00, 0xb8, // str w3, [x0, #4]! // Basically PSH w3
    };
    
    memcpy(jc->native_code + jc->native_size, add, sizeof(add));
    jc->native_size += sizeof(add);

    return;
}

void emit_ret(jitc* jc) {
    if (DEBUG) {
        printf("JIT RET\n");
    }

    unsigned char ret[] = {
        0xc0, 0x03, 0x5f, 0xd6  // ret
    };
    
    memcpy(jc->native_code + jc->native_size, ret, sizeof(ret));
    jc->native_size += sizeof(ret);

    return;
}


#elif defined(__x86_64__)

void emit_psh(unsigned char* native_code, size_t* snippet_size, int val)  {
    return;
}

void emit_add(vm_state* state) {
    // Machine code for add_two (from Section 4):
    unsigned char machine_code[] = {
        0x8B, 0xC7,             // mov eax, edi  (first arg)
        0x03, 0xC6,             // add eax, esi  (second arg)
        0xC3                    // ret
    };
    size_t code_size = sizeof(machine_code);

    write_native_code(state, machine_code, code_size);

    return;
}

#endif

void emit_native_bytes(jitc* jc) {
    // Setup jit emission instructions
    emit_function emit[256];

    emit[OP_PSH] = emit_psh;
    emit[OP_ADD] = emit_add;

    for (; jc->bytecode[jc->bytecode_index] != 0; jc->bytecode_index++) {
        int bytecode_instr = jc->bytecode[jc->bytecode_index];
        emit[bytecode_instr](jc);
    }
    printf("Emitted main instructions...\n");
    emit_ret(jc);
}