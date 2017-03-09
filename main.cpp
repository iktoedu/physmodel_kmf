#include <iostream>
#include <iomanip>
#include "core/core_types.h"
#include "core_2d/core_2d_atom_reference.h"
#include "core_2d/core_2d_util.h"
#include "simple_2d/simple_2d_atom_neighbour_iterator.h"

using namespace std;

#define SIZE_X   5
#define SIZE_Y   10

int main(int argc, char *argv[])
{
    btAtomValue **field = core_2d_allocate_field(SIZE_X, SIZE_Y);

    srand(time(NULL));
    core_2d_generate_field_values(field, SIZE_X, SIZE_Y, [](btPositionValue x, btPositionValue y) {
        return (btAtomValue)(rand() % 100) / 100;
    });

    for (btPositionValue y = 0; y < SIZE_Y; y++) {
        for (btPositionValue x = 0; x < SIZE_X; x++) {
            cout << setfill(' ') << setw(10);
            cout << setprecision(2) << fixed << field[y][x];
        }
        cout << endl;
    }

    atomReference2D ref;
    ref.field = field;
    ref.x = 0;
    ref.y = 0;

    cout << setprecision(2) << fixed << CORE_2D_RESOLVE_ATOM_REFERENCE(ref) << endl;

    AtomNeighbourIterator it(ref, SIZE_X, SIZE_Y);
    for (int i = 0; i < 4; i++, it++) {
        cout << setprecision(2) << fixed << CORE_2D_RESOLVE_ATOM_REFERENCE(*it) << endl;

        AtomNeighbourIterator innerIt(*it, SIZE_X, SIZE_Y);
        for (int j = 0; j < 4; j++, innerIt++) {
            cout << setfill(' ') << setw(10);
            cout << setprecision(2) << fixed << CORE_2D_RESOLVE_ATOM_REFERENCE(*innerIt);
        }
        cout << endl;
    }

    core_2d_deallocate_field(field, SIZE_X, SIZE_Y);

    return 0;
}
