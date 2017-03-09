#ifndef CORE_2D_UTIL_H
#define CORE_2D_UTIL_H

#include "core/core_types.h"

btAtomValue **core_2d_allocate_field(btPositionValue, btPositionValue);
void core_2d_deallocate_field(btAtomValue **, btPositionValue, btPositionValue);

#endif // CORE_2D_UTIL_H
