#ifndef SIMPLE_2D_ATOM_NEIGHBOUR_ITERATOR_H
#define SIMPLE_2D_ATOM_NEIGHBOUR_ITERATOR_H

#include <iterator>
#include "core_2d/core_2d_atom_reference.h"

namespace Simple2D {

class AtomNeighbourIterator : public std::iterator<std::input_iterator_tag, atom_reference_2d_t>
{
public:

    AtomNeighbourIterator(const atom_reference_2d_t &, position_value_t, position_value_t);
    AtomNeighbourIterator(const AtomNeighbourIterator &);

    ~AtomNeighbourIterator();

    AtomNeighbourIterator &operator ++();
    AtomNeighbourIterator operator ++(int);

    bool operator ==(const AtomNeighbourIterator &);
    bool operator !=(const AtomNeighbourIterator &);

    atom_reference_2d_t &operator *();

    // Checks whether we are at the end
    // Works much faster rather than comparison with "end" iterator
    bool atEnd();

    // Move internal pointer to the start
    // Allows to re-use one iterator object
    void reset();

private:
    atom_reference_2d_t startReference;
    atom_reference_2d_t *neighbourReferences;
    int currentPosition;

    position_value_t loopSizeX;
    position_value_t loopSizeY;

    void init();
};

} // namespace Simple2D

#endif // SIMPLE_2D_ATOM_NEIGHBOUR_ITERATOR_H
