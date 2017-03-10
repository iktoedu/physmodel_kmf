#ifndef SIMPLE_2D_ATOM_GRID_ITERATOR_H
#define SIMPLE_2D_ATOM_GRID_ITERATOR_H

#include <iterator>
#include "core_2d/core_2d_atom_reference.h"

namespace Simple2D {

class AtomGridIterator : public std::iterator<std::input_iterator_tag, atomReference2D>
{
public:

    static AtomGridIterator endInterator(btAtomValue **, btPositionValue, btPositionValue);

    AtomGridIterator(btAtomValue **, btPositionValue, btPositionValue);
    AtomGridIterator(const AtomGridIterator &);

    AtomGridIterator &operator ++();
    AtomGridIterator operator ++(int);

    bool operator ==(const AtomGridIterator &);
    bool operator !=(const AtomGridIterator &);

    atomReference2D operator *();

private:
    btAtomValue **mvpField;

    btPositionValue mvSizeX;
    btPositionValue mvSizeY;

    btPositionValue mvPositionX;
    btPositionValue mvPositionY;
};

} // namespace Simple2D

#endif // SIMPLE_2D_ATOM_GRID_ITERATOR_H
