#include "simple_2d_atom_grid_iterator.h"

namespace Simple2D {

AtomGridIterator::AtomGridIterator(btAtomValue **field, btPositionValue sizeX, btPositionValue sizeY)
    : mvpField(field), mvSizeX(sizeX), mvSizeY(sizeY)
{
    mvPositionX = 0;
    mvPositionY = 0;
}

AtomGridIterator::AtomGridIterator(const AtomGridIterator &iterator)
    : mvpField(iterator.mvpField), mvSizeX(iterator.mvSizeX), mvSizeY(iterator.mvSizeY), mvPositionX(iterator.mvPositionX), mvPositionY(iterator.mvPositionY)
{
}

AtomGridIterator &AtomGridIterator::operator ++()
{
    ++mvPositionX;
    if (mvPositionX >= mvSizeX) {
        mvPositionX = 0;
        ++mvPositionY;
    }

    return *this;
}

bool AtomGridIterator::operator ==(const AtomGridIterator &iterator)
{
    return (
                (mvpField == iterator.mvpField)
                && (mvSizeX == iterator.mvSizeX)
                && (mvSizeY == iterator.mvSizeY)
                && (mvPositionX == iterator.mvPositionX)
                && (mvPositionY == iterator.mvPositionY)
    );
}

bool AtomGridIterator::operator !=(const AtomGridIterator &iterator)
{
    return ! operator ==(iterator);
}

AtomGridIterator AtomGridIterator::operator ++(int)
{
    AtomGridIterator current(*this);
    operator ++();

    return current;
}

atomReference2D AtomGridIterator::operator *()
{
    atomReference2D ref;
    ref.field = mvpField;
    ref.x = mvPositionX;
    ref.y = mvPositionY;

    return ref;
}

} // namespace Simple2D
