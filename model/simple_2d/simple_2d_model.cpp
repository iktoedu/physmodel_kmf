#include "simple_2d_model.h"
#include "core_2d/core_2d_util.h"

#include <cmath>

namespace Simple2D {

Model::Model(position_value_t sizeX, position_value_t sizeY)
{
    mvSettings.sizeX = sizeX;
    mvSettings.sizeY = sizeY;
    allocateData();
    mvIsInitialized = false;
}

Model::Model(model_settigns_t settings, model_state_t state, atom_value_t **data)
    : mvSettings(settings), mvState(state), mvpData(data)
{
    allocateTemporalShadowData();
    allocateTemporalDirectSumData();
    allocateTemporalReverseSumData();
    mvIsInitialized = true;
}

Model::~Model()
{
    if (mvpAtomNeighbourIteratorsLv2) {
        deallocateNeighbourIterators(mvpAtomNeighbourIteratorsLv2);
        mvpAtomNeighbourIteratorsLv2 = 0;
    }

    if (mvpAtomNeighbourIteratorsLv1) {
        deallocateNeighbourIterators(mvpAtomNeighbourIteratorsLv1);
        mvpAtomNeighbourIteratorsLv1 = 0;
    }

    if (mvpAtomGridIterator) {
        delete mvpAtomGridIterator;
        mvpAtomGridIterator = 0;
    }

    if (mvpTemporalReverseSumData) {
        core_2d_deallocate_field(mvpTemporalReverseSumData, mvSettings.sizeX, mvSettings.sizeY);
    }
    if (mvpTemporalDirectSumData) {
        core_2d_deallocate_field(mvpTemporalDirectSumData, mvSettings.sizeX, mvSettings.sizeY);
    }
    if (mvpTemporalData) {
        core_2d_deallocate_field(mvpTemporalData, mvSettings.sizeX, mvSettings.sizeY);
    }
    core_2d_deallocate_field(mvpData, mvSettings.sizeX, mvSettings.sizeY);
}

void Model::init(double tStart, double tEnd, double tStep, double temperature)
{
    mvSettings.tStart       = tStart;
    mvSettings.tEnd         = tEnd;
    mvSettings.tStep        = tStep;
    mvSettings.temperature  = temperature;

    mvState.tCurrent    = mvSettings.tStart;

    allocateTemporalShadowData();
    allocateTemporalDirectSumData();
    allocateTemporalReverseSumData();
    mvIsInitialized     = true;
}

bool Model::isInitialized()
{
    return mvIsInitialized;
}

void Model::think()
{
    AtomGridIterator &it = getAtomGridIterator();

    // Calculating sums of neighbour values
    // They'll be used further
    // This should improve the performance
    for (; !it.atEnd(); ++it) {
        atom_reference_2d_t atom = *it;

        mvpTemporalDirectSumData[atom.y][atom.x]    = 0;
        mvpTemporalReverseSumData[atom.y][atom.x]   = 0;

        AtomNeighbourIterator &itNeighbour = getAtomNeighbourLv1Iterator(atom);
        for (; !itNeighbour.atEnd(); ++itNeighbour) {
            mvpTemporalDirectSumData[atom.y][atom.x] += CORE_2D_RESOLVE_ATOM_REFERENCE(*itNeighbour);
            mvpTemporalReverseSumData[atom.y][atom.x] += (1 - CORE_2D_RESOLVE_ATOM_REFERENCE(*itNeighbour));
        }
    }

    // Move to the start
    it.reset();

    // Calculate new state
    for (; !it.atEnd(); ++it) {
        atom_reference_2d_t atom = *it;

        mvpTemporalData[atom.y][atom.x] = CORE_2D_RESOLVE_ATOM_REFERENCE(atom) + getAtomDelta(atom) * mvSettings.tStep;
    }

    // Flush new state to buffer
    {
        atom_value_t **tmp = mvpData;
        mvpData = mvpTemporalData;
        mvpTemporalData = tmp;
    }

    mvState.tCurrent += mvSettings.tStep;
}

model_settigns_t Model::getModelSettings()
{
    return mvSettings;
}

model_state_t Model::getModelState()
{
    return mvState;
}

atom_value_t **Model::getDataPointer()
{
    return mvpData;
}

progress_unit_t Model::getTotalSteps()
{
    return floor((mvSettings.tEnd - mvSettings.tStart) / mvSettings.tStep);
}

progress_unit_t Model::getCurrentStep()
{
    return floor((mvState.tCurrent - mvSettings.tStart) / mvSettings.tStep);
}

bool Model::isModellingEnded()
{
    return !(mvState.tCurrent < mvSettings.tEnd);
}

void Model::allocateData()
{
    mvpData = core_2d_allocate_field(mvSettings.sizeX, mvSettings.sizeY);
}

void Model::allocateTemporalShadowData()
{
    mvpTemporalData = core_2d_allocate_field(mvSettings.sizeX, mvSettings.sizeY);
}

void Model::allocateTemporalDirectSumData()
{
    mvpTemporalDirectSumData = core_2d_allocate_field(mvSettings.sizeX, mvSettings.sizeY);
}

void Model::allocateTemporalReverseSumData()
{
    mvpTemporalReverseSumData = core_2d_allocate_field(mvSettings.sizeX, mvSettings.sizeY);
}

atom_value_t Model::getAtomDelta(atom_reference_2d_t &atom)
{
    double sumGammaLeft     = 0;
    double sumGammaRight    = 0;

    AtomNeighbourIterator &it = getAtomNeighbourLv1Iterator(atom);

    for (; !it.atEnd(); ++it) {
        sumGammaLeft += (1 - CORE_2D_RESOLVE_ATOM_REFERENCE(*it)) * getAtomExProb(atom, *it);
        sumGammaRight += CORE_2D_RESOLVE_ATOM_REFERENCE(*it) * getAtomExProb(*it, atom);
    }

    return (-CORE_2D_RESOLVE_ATOM_REFERENCE(atom)) * sumGammaLeft + (1 - CORE_2D_RESOLVE_ATOM_REFERENCE(atom)) * sumGammaRight;
}

double Model::getAtomExProb(atom_reference_2d_t &a, atom_reference_2d_t &b)
{
    // TODO Implement Gamma
    return 0;
}

atom_value_t Model::getNeighboursDirectSum(atom_reference_2d_t &atom)
{
    return mvpTemporalDirectSumData[atom.y][atom.x];
}

atom_value_t Model::getNeighboursReverseSum(atom_reference_2d_t &atom)
{
    return mvpTemporalReverseSumData[atom.y][atom.x];
}

AtomGridIterator &Model::getAtomGridIterator()
{
    if (!mvpAtomGridIterator) {
        mvpAtomGridIterator = new AtomGridIterator(mvpData, mvSettings.sizeX, mvSettings.sizeY);
    }
    else {
        mvpAtomGridIterator->reset();
    }

    return *mvpAtomGridIterator;
}

AtomNeighbourIterator ***Model::allocateNeighbourIterators()
{
    AtomNeighbourIterator ***p = new AtomNeighbourIterator**[mvSettings.sizeY];
    for (position_value_t y = 0; y < mvSettings.sizeY; ++y) {
        p[y] = new AtomNeighbourIterator*[mvSettings.sizeX];
        for (position_value_t x = 0; x < mvSettings.sizeX; ++x) {
            atom_reference_2d_t ref;
            ref.field = mvpData;
            ref.x = x;
            ref.y = y;
            p[y][x] = new AtomNeighbourIterator(ref, mvSettings.sizeX, mvSettings.sizeY);
        }
    }

    return p;
}

void Model::deallocateNeighbourIterators(AtomNeighbourIterator ***p)
{
    for (position_value_t y = 0; y < mvSettings.sizeY; ++y) {
        for (position_value_t x = 0; x < mvSettings.sizeX; ++x) {
            delete p[y][x];
        }
        delete [] p[y];
    }
    delete [] p;
}

AtomNeighbourIterator &Model::getAtomNeighbourLv1Iterator(const atom_reference_2d_t &atom)
{
    // TODO Check main pointer

    if (!mvpAtomNeighbourIteratorsLv1) {
        mvpAtomNeighbourIteratorsLv1 = allocateNeighbourIterators();
    }

    AtomNeighbourIterator *iterator = mvpAtomNeighbourIteratorsLv1[atom.y][atom.x];
    iterator->reset();

    return *iterator;
}

AtomNeighbourIterator &Model::getAtomNeighbourLv2Iterator(const atom_reference_2d_t &atom)
{
    // TODO Check main pointer

    if (!mvpAtomNeighbourIteratorsLv2) {
        mvpAtomNeighbourIteratorsLv2 = allocateNeighbourIterators();
    }

    AtomNeighbourIterator *iterator = mvpAtomNeighbourIteratorsLv2[atom.y][atom.x];
    iterator->reset();

    return *iterator;
}

} // namespace Simple2D
