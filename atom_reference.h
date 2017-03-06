#ifndef ATOM_REFERENCE_H
#define ATOM_REFERENCE_H

#include "basic_types.h"

#define RESOLVE_ATOM_REF(__ref) (__ref).field[(__ref).y][(__ref).x]

typedef struct {
    btAtomValue **field;
    btPositionValue x;
    btPositionValue y;
} atomReference;

#endif // ATOM_REFERENCE_H
