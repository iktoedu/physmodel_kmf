#include "atom_neighbour_iterator.h"

AtomNeighbourIterator::AtomNeighbourIterator(const atomReference &inputReference, btPositionValue loopSizeX, btPositionValue loopSizeY)
    : startReference(inputReference), loopSizeX(loopSizeX), loopSizeY(loopSizeY)
{
    init();
}

AtomNeighbourIterator::AtomNeighbourIterator(const AtomNeighbourIterator &sourceIterator, btPositionValue loopSizeX, btPositionValue loopSizeY)
    : startReference(sourceIterator.startReference), loopSizeX(loopSizeX), loopSizeY(loopSizeY)
{
    init();
}

void AtomNeighbourIterator::init()
{
    neighbourReferences = new atomReference[4];

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

atomReference &AtomNeighbourIterator::operator *()
{
    return neighbourReferences[currentPosition];
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
