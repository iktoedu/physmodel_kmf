#include "core_2d_util.h"

atom_value_t **core_2d_allocate_field(position_value_t sizeX, position_value_t sizeY)
{
    atom_value_t **field = new atom_value_t*[sizeY];
    for (position_value_t y = 0; y < sizeY; y++) {
        field[y] = new atom_value_t[sizeX];
    }

    return field;
}

void core_2d_deallocate_field(atom_value_t **field, position_value_t sizeX, position_value_t sizeY)
{
    for (position_value_t y = 0; y < sizeY; y++) {
        delete [] field[y];
    }
    delete [] field;
}

void core_2d_generate_field_values(atom_value_t **field, position_value_t sizeX, position_value_t sizeY, std::function<atom_value_t (position_value_t, position_value_t)> callback)
{
    for (position_value_t y = 0; y < sizeY; y++) {
        for (position_value_t x = 0; x < sizeX; x++) {
            field[y][x] = callback(x, y);
        }
    }
}
