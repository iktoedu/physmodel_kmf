#ifndef CORE_2D_UTIL_H
#define CORE_2D_UTIL_H

#include <functional>
#include "core/core_types.h"

atom_value_t **core_2d_allocate_field(position_value_t, position_value_t);
void core_2d_deallocate_field(atom_value_t **, position_value_t, position_value_t);
void core_2d_generate_field_values(atom_value_t **, position_value_t, position_value_t, std::function<atom_value_t (position_value_t, position_value_t)>);

#endif // CORE_2D_UTIL_H
