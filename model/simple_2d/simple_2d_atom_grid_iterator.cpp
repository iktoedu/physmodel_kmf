#include "simple_2d_atom_grid_iterator.h"

namespace Simple2D {

AtomGridIterator::AtomGridIterator(atom_value_t **field, position_value_t sizeX, position_value_t sizeY)
    : mvpField(field), mvSizeX(sizeX), mvSizeY(sizeY)
{
    mvPositionX = 0;
    mvPositionY = 0;
}

AtomGridIterator::AtomGridIterator(const AtomGridIterator &iterator)
    : mvpField(iterator.mvpField), mvSizeX(iterator.mvSizeX), mvSizeY(iterator.mvSizeY), mvPositionX(iterator.mvPositionX), mvPositionY(iterator.mvPositionY)
{
}

AtomGridIterator AtomGridIterator::endIterator(atom_value_t **field, position_value_t sizeX, position_value_t sizeY)
{
    AtomGridIterator iterator(field, sizeX, sizeY);
    iterator.mvPositionX = 0;
    iterator.mvPositionY = iterator.mvSizeY;

    return iterator;
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

atom_reference_2d_t AtomGridIterator::operator *()
{
    atom_reference_2d_t ref;
    ref.field = mvpField;
    ref.x = mvPositionX;
    ref.y = mvPositionY;

    return ref;
}

void AtomGridIterator::reset()
{
    mvPositionX = 0;
    mvPositionY = 0;
}

} // namespace Simple2D
