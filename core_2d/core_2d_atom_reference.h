#ifndef CORE_2D_ATOM_REFERENCE_H
#define CORE_2D_ATOM_REFERENCE_H

#include "core/core_types.h"

#define RESOLVE_ATOM_REF(__ref) (__ref).field[(__ref).y][(__ref).x]

typedef struct {
    btAtomValue **field;
    btPositionValue x;
    btPositionValue y;
} atomReference;

#endif // CORE_2D_ATOM_REFERENCE_H
