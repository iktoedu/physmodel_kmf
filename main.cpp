#include <iostream>
#include <iomanip>
#include "basic_types.h"
#include "atom_reference.h"
#include "atom_neighbour_iterator.h"

using namespace std;

#define SIZE_X   5
#define SIZE_Y   10

int main(int argc, char *argv[])
{
    btAtomValue **field;
    field = new btAtomValue*[SIZE_Y];
    for (btPositionValue y = 0; y < SIZE_Y; y++) {
        field[y] = new btAtomValue[SIZE_X];
    }

    srand(time(NULL));
    for (btPositionValue y = 0; y < SIZE_Y; y++) {
        for (btPositionValue x = 0; x < SIZE_X; x++) {
            field[y][x] = (btAtomValue)(rand() % 100) / 100;
        }
    }

    for (btPositionValue y = 0; y < SIZE_Y; y++) {
        for (btPositionValue x = 0; x < SIZE_X; x++) {
            cout << setfill(' ') << setw(10);
            cout << setprecision(2) << fixed << field[y][x];
        }
        cout << endl;
    }

    atomReference ref;
    ref.field = field;
    ref.x = 2;
    ref.y = 5;

    cout << setprecision(2) << fixed << RESOLVE_ATOM_REF(ref) << endl;

    AtomNeighbourIterator it(ref);
    for (int i = 0; i < 4; i++, it++) {
        cout << setprecision(2) << fixed << RESOLVE_ATOM_REF(*it) << endl;

        AtomNeighbourIterator innerIt(*it);
        for (int j = 0; j < 4; j++, innerIt++) {
            cout << setfill(' ') << setw(10);
            cout << setprecision(2) << fixed << RESOLVE_ATOM_REF(*innerIt);
        }
        cout << endl;
    }

    for (btPositionValue y = 0; y < SIZE_Y; y++) {
        delete [] field[y];
    }
    delete [] field;

    return 0;
}
