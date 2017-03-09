#ifndef SIMPLE_2D_ATOM_NEIGHBOUR_ITERATOR_H
#define SIMPLE_2D_ATOM_NEIGHBOUR_ITERATOR_H

#include <iterator>
#include "core_2d/core_2d_atom_reference.h"

namespace Simple2D {

class AtomNeighbourIterator : public std::iterator<std::input_iterator_tag, atomReference2D>
{
public:
    AtomNeighbourIterator(const atomReference2D &, btPositionValue, btPositionValue);
    AtomNeighbourIterator(const AtomNeighbourIterator &, btPositionValue, btPositionValue);

    AtomNeighbourIterator &operator ++();
    AtomNeighbourIterator operator ++(int);

    bool operator ==(const AtomNeighbourIterator &);
    bool operator !=(const AtomNeighbourIterator &);

    atomReference2D &operator *();

private:
    atomReference2D startReference;
    atomReference2D *neighbourReferences;
    int currentPosition;

    btPositionValue loopSizeX;
    btPositionValue loopSizeY;

    void init();
};

} // namespace Simple2D

#endif // SIMPLE_2D_ATOM_NEIGHBOUR_ITERATOR_H
