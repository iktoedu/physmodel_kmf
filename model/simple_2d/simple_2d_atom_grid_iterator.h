#ifndef SIMPLE_2D_ATOM_GRID_ITERATOR_H
#define SIMPLE_2D_ATOM_GRID_ITERATOR_H

#include <iterator>
#include "core_2d/core_2d_atom_reference.h"

namespace Simple2D {

class AtomGridIterator : public std::iterator<std::input_iterator_tag, atom_reference_2d_t>
{
public:

    static AtomGridIterator endIterator(atom_value_t **, position_value_t, position_value_t);

    AtomGridIterator(atom_value_t **, position_value_t, position_value_t);
    AtomGridIterator(const AtomGridIterator &);

    AtomGridIterator &operator ++();
    AtomGridIterator operator ++(int);

    bool operator ==(const AtomGridIterator &);
    bool operator !=(const AtomGridIterator &);

    atom_reference_2d_t operator *();

    void reset();

private:
    atom_value_t **mvpField;

    position_value_t mvSizeX;
    position_value_t mvSizeY;

    position_value_t mvPositionX;
    position_value_t mvPositionY;
};

} // namespace Simple2D

#endif // SIMPLE_2D_ATOM_GRID_ITERATOR_H
