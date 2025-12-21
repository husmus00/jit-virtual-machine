#ifndef TYPES_H
#define TYPES_H

#include "config.h"

typedef struct label label;

struct label {
    char name[LABEL_MAX_LENGTH];
    int address;        // Bytecode address
    int is_native;      
    int native_address; // Real memory address if native
};

#endif // TYPES_H
