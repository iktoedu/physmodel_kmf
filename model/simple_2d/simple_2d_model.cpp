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
    allocateShadowData();
    mvIsInitialized = true;
}

Model::~Model()
{
    if (mvpAtomGridIterator) {
        delete mvpAtomGridIterator;
        mvpAtomGridIterator = 0;
    }

    if (mvpAtomGridEndIterator) {
        delete mvpAtomGridEndIterator;
        mvpAtomGridEndIterator = 0;
    }

    if (mvpShadowData) {
        core_2d_deallocate_field(mvpShadowData, mvSettings.sizeX, mvSettings.sizeY);
    }
    core_2d_deallocate_field(mvpData, mvSettings.sizeX, mvSettings.sizeY);
}

void Model::init(double tStart, double tEnd, double tStep)
{
    mvSettings.tStart   = tStart;
    mvSettings.tEnd     = tEnd;
    mvSettings.tStep    = tStep;

    mvState.tCurrent    = mvSettings.tStart;

    allocateShadowData();
    mvIsInitialized     = true;
}

bool Model::isInitialized()
{
    return mvIsInitialized;
}

void Model::think()
{
    AtomGridIterator it = getAtomGridIterator();
    AtomGridIterator end = getAtomGridEndIterator();

    for (; it != end; ++it) {
        atom_reference_2d_t atom = *it;

        mvpShadowData[atom.y][atom.x] = CORE_2D_RESOLVE_ATOM_REFERENCE(atom) + atomDelta(atom) * mvSettings.tStep;
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

void Model::allocateShadowData()
{
    mvpShadowData = core_2d_allocate_field(mvSettings.sizeX, mvSettings.sizeY);
}

atom_value_t Model::atomDelta(atom_reference_2d_t &atom)
{
    double sumGammaLeft     = 0;
    double sumGammaRight    = 0;

    AtomNeighbourIterator it(atom, mvSettings.sizeX, mvSettings.sizeY);
    AtomNeighbourIterator end = AtomNeighbourIterator::endIterator(atom, mvSettings.sizeX, mvSettings.sizeY);

    for (; it != end; ++it) {
        sumGammaLeft += (1 - CORE_2D_RESOLVE_ATOM_REFERENCE(*it)) * atomExchangeFrequency(atom, *it);
        sumGammaRight += CORE_2D_RESOLVE_ATOM_REFERENCE(*it) * atomExchangeFrequency(*it, atom);
    }

    return (-CORE_2D_RESOLVE_ATOM_REFERENCE(atom)) * sumGammaLeft + (1 - CORE_2D_RESOLVE_ATOM_REFERENCE(atom)) * sumGammaRight;
}

double Model::atomExchangeFrequency(atom_reference_2d_t &a, atom_reference_2d_t &b)
{
    // TODO Implement Gamma
    return 0;
}

AtomGridIterator &Model::getAtomGridIterator()
{
    if (!mvpAtomGridIterator) {
        mvpAtomGridIterator = new AtomGridIterator(mvpData, mvSettings.sizeX, mvSettings.sizeY);
    }
    mvpAtomGridIterator->reset();

    return *mvpAtomGridIterator;
}

AtomGridIterator &Model::getAtomGridEndIterator()
{
    if (!mvpAtomGridEndIterator) {
        mvpAtomGridEndIterator = AtomGridIterator::endIteratorPointer(mvpData, mvSettings.sizeX, mvSettings.sizeY);
    }

    return *mvpAtomGridEndIterator;
}

} // namespace Simple2D
