#include "core_2d_util.h"

btAtomValue **core_2d_allocate_field(btPositionValue sizeX, btPositionValue sizeY)
{
    btAtomValue **field = new btAtomValue*[sizeY];
    for (btPositionValue y = 0; y < sizeY; y++) {
        field[y] = new btAtomValue[sizeX];
    }

    return field;
}

void core_2d_deallocate_field(btAtomValue **field, btPositionValue sizeX, btPositionValue sizeY)
{
    for (btPositionValue y = 0; y < sizeY; y++) {
        delete [] field[y];
    }
    delete [] field;
}
