#include <iostream>
#include <iomanip>
#include "core/core_types.h"
#include "core_2d/core_2d_atom_reference.h"
#include "core_2d/core_2d_util.h"
#include "simple_2d/simple_2d_atom_neighbour_iterator.h"
#include "simple_2d/simple_2d_atom_grid_iterator.h"

using namespace std;
using namespace Simple2D;

#define SIZE_X   5
#define SIZE_Y   10

int main(int argc, char *argv[])
{
    atom_value_t **field = core_2d_allocate_field(SIZE_X, SIZE_Y);

    srand(time(NULL));
    core_2d_generate_field_values(field, SIZE_X, SIZE_Y, [](position_value_t x, position_value_t y) {
        return (atom_value_t)(rand() % 100) / 100;
    });

    for (position_value_t y = 0; y < SIZE_Y; y++) {
        for (position_value_t x = 0; x < SIZE_X; x++) {
            cout << setfill(' ') << setw(10);
            cout << setprecision(2) << fixed << field[y][x];
        }
        cout << endl;
    }

    atom_reference_2d_t ref;
    ref.field = field;
    ref.x = 0;
    ref.y = 0;

    cout << setprecision(2) << fixed << CORE_2D_RESOLVE_ATOM_REFERENCE(ref) << endl;

    AtomNeighbourIterator it(ref, SIZE_X, SIZE_Y);
    AtomNeighbourIterator end = AtomNeighbourIterator::endIterator(ref, SIZE_X, SIZE_Y);
    for (; it != end; ++it) {
        cout << setprecision(2) << fixed << CORE_2D_RESOLVE_ATOM_REFERENCE(*it) << endl;

        AtomNeighbourIterator innerIt(*it, SIZE_X, SIZE_Y);
        AtomNeighbourIterator innerEnd = AtomNeighbourIterator::endIterator(*it, SIZE_X, SIZE_Y);
        for (; innerIt != innerEnd; ++innerIt) {
            cout << setfill(' ') << setw(10);
            cout << setprecision(2) << fixed << CORE_2D_RESOLVE_ATOM_REFERENCE(*innerIt);
        }
        cout << endl;
    }

    {
        AtomGridIterator it(field, SIZE_X, SIZE_Y);
        AtomGridIterator end = AtomGridIterator::endIterator(field, SIZE_X, SIZE_Y);
        for (; it != end; ++it) {
            cout << setfill(' ') << setw(10) << setprecision(2) << fixed << CORE_2D_RESOLVE_ATOM_REFERENCE(*it);
        }
    }

    core_2d_deallocate_field(field, SIZE_X, SIZE_Y);

    return 0;
}
