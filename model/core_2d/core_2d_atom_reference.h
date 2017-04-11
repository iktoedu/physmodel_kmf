#ifndef CORE_2D_ATOM_REFERENCE_H
#define CORE_2D_ATOM_REFERENCE_H

#include "core/core_types.h"

#define CORE_2D_RESOLVE_ATOM_REFERENCE(__ref) (__ref).field[(__ref).y][(__ref).x]

typedef struct {
    atom_value_t **field;
    position_value_t x;
    position_value_t y;
} atom_reference_2d_t;

#endif // CORE_2D_ATOM_REFERENCE_H
