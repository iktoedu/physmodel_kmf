#ifndef ATOM_NEIGHBOUR_ITERATOR_H
#define ATOM_NEIGHBOUR_ITERATOR_H

#include <iterator>
#include "atom_reference.h"

class AtomNeighbourIterator : public std::iterator<std::input_iterator_tag, atomReference>
{
public:
    AtomNeighbourIterator(const atomReference &, btPositionValue, btPositionValue);
    AtomNeighbourIterator(const AtomNeighbourIterator &, btPositionValue, btPositionValue);

    AtomNeighbourIterator &operator ++();
    AtomNeighbourIterator operator ++(int);

//    bool operator==(const AtomNeighbourIterator &);
//    bool operator!=(const AtomNeighbourIterator &);

    atomReference &operator *();

private:
    atomReference startReference;
    atomReference *neighbourReferences;
    int currentPosition;

    btPositionValue loopSizeX;
    btPositionValue loopSizeY;

    void init();
};

#endif // ATOM_NEIGHBOUR_ITERATOR_H
