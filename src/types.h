#ifndef TYPES_H
#define TYPES_H

#include "config.h"

struct label {
    char name[LABEL_MAX_LENGTH];
    int address;
};

#endif // TYPES_H
