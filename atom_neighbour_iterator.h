#ifndef ATOM_NEIGHBOUR_ITERATOR_H
#define ATOM_NEIGHBOUR_ITERATOR_H

#include <iterator>
#include "atom_reference.h"

class AtomNeighbourIterator : public std::iterator<std::input_iterator_tag, atomReference>
{
public:
    AtomNeighbourIterator(atomReference &inputReference);
    AtomNeighbourIterator(const AtomNeighbourIterator &);

    AtomNeighbourIterator &operator ++();
    AtomNeighbourIterator operator ++(int);

//    bool operator==(const AtomNeighbourIterator &);
//    bool operator!=(const AtomNeighbourIterator &);

    atomReference &operator *();

private:
    atomReference startReference;
    atomReference *neighbourReferences;
    int currentPosition;

    void init();
};

#endif // ATOM_NEIGHBOUR_ITERATOR_H
