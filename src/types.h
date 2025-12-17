#ifndef TYPES_H
#define TYPES_H

#include "config.h"

typedef struct label label;

struct label {
    char name[LABEL_MAX_LENGTH];
    int address;
    int native;
};

#endif // TYPES_H
