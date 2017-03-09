#include "simple_2d/simple_2d_atom_neighbour_iterator.h"

namespace Simple2D {

AtomNeighbourIterator::AtomNeighbourIterator(const atomReference2D &inputReference, btPositionValue loopSizeX, btPositionValue loopSizeY)
    : startReference(inputReference), loopSizeX(loopSizeX), loopSizeY(loopSizeY)
{
    init();
}

AtomNeighbourIterator::AtomNeighbourIterator(const AtomNeighbourIterator &sourceIterator, btPositionValue loopSizeX, btPositionValue loopSizeY)
    : loopSizeX(loopSizeX), loopSizeY(loopSizeY)
{
    startReference = sourceIterator.neighbourReferences[sourceIterator.currentPosition];
    init();
}

void AtomNeighbourIterator::init()
{
    neighbourReferences = new atomReference2D[4];

    neighbourReferences[0].field = startReference.field;
    neighbourReferences[0].x = startReference.x;
    neighbourReferences[0].y = ((startReference.y <= 0) ? (loopSizeY - 1) : (startReference.y - 1));

    neighbourReferences[1].field = startReference.field;
    neighbourReferences[1].x = ((startReference.x >= loopSizeX - 1) ? 0 : (startReference.x + 1));
    neighbourReferences[1].y = startReference.y;

    neighbourReferences[2].field = startReference.field;
    neighbourReferences[2].x = startReference.x;
    neighbourReferences[2].y = ((startReference.y >= loopSizeY - 1) ? 0 : (startReference.y + 1));

    neighbourReferences[3].field = startReference.field;
    neighbourReferences[3].x = ((startReference.x <= 0) ? (loopSizeX - 1) : (startReference.x - 1));
    neighbourReferences[3].y = startReference.y;

    currentPosition = 0;
}

atomReference2D &AtomNeighbourIterator::operator *()
{
    return neighbourReferences[currentPosition];
}

bool AtomNeighbourIterator::operator ==(const AtomNeighbourIterator &iterator)
{
    return (
                (startReference.field == iterator.startReference.field)
                && (startReference.x == iterator.startReference.x)
                && (startReference.y == iterator.startReference.y)
                && (loopSizeX == iterator.loopSizeX)
                && (loopSizeY == iterator.loopSizeY)
                && (currentPosition == iterator.currentPosition)
    );
}

bool AtomNeighbourIterator::operator !=(const AtomNeighbourIterator &iterator)
{
    return ! operator ==(iterator);
}

AtomNeighbourIterator &AtomNeighbourIterator::operator ++()
{
    currentPosition++;
    return *this;
}

AtomNeighbourIterator AtomNeighbourIterator::operator ++(int)
{
    AtomNeighbourIterator current(*this);
    operator ++();

    return current;
}

} // namespace Simple2D
