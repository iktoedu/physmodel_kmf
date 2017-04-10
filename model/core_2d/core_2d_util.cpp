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

void core_2d_generate_field_values(btAtomValue **field, btPositionValue sizeX, btPositionValue sizeY, std::function<btAtomValue (btPositionValue, btPositionValue)> callback)
{
    for (btPositionValue y = 0; y < sizeY; y++) {
        for (btPositionValue x = 0; x < sizeX; x++) {
            field[y][x] = callback(x, y);
        }
    }
}
