#include "simple_2d_model.h"
#include "core_2d/core_2d_util.h"
#include "basis/physics_constant.h"

#include <cmath>
#include <exception>
#include <iomanip>

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
    initTemporalVariables();
    mvIsInitialized = true;
}

Model::~Model()
{
    if (mvpAtomNeighbourIteratorsLv1) {
        deallocateNeighbourIterators(mvpAtomNeighbourIteratorsLv1);
        mvpAtomNeighbourIteratorsLv1 = 0;
    }

    if (mvpAtomGridIterator) {
        delete mvpAtomGridIterator;
        mvpAtomGridIterator = 0;
    }

    cleanupTemporalVariables();
    core_2d_deallocate_field(mvpData, mvSettings.sizeX, mvSettings.sizeY);
}

void Model::init(double tStart, double tEnd, double tStep, double temperature)
{
    mvSettings.tStart       = tStart;
    mvSettings.tEnd         = tEnd;
    mvSettings.tStep        = tStep;
    mvSettings.temperature  = temperature;

    mvState.tCurrent    = mvSettings.tStart;

    initTemporalVariables();
    mvIsInitialized     = true;
}

bool Model::isInitialized()
{
    return mvIsInitialized;
}

void Model::think()
{
    preThink();

    AtomGridIterator &it = getAtomGridIterator();

    // Calculate new state
    for (; !it.atEnd(); ++it) {
        atom_reference_2d_t atom = *it;

        mvpTemporalData[atom.y][atom.x] = CORE_2D_RESOLVE_ATOM_REFERENCE(atom) + getAtomDelta(atom) * mvSettings.tStep;
    }

    postThink();
}

void Model::preThink()
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

        mvpTemporalDirectSumData[atom.y][atom.x] /= PHYSICS_CONSTANT_EV;
        mvpTemporalReverseSumData[atom.y][atom.x] /= PHYSICS_CONSTANT_EV;
    }
}

void Model::postThink()
{
    // Flush new state to buffer
    // Exchanging row by row is required here
    // Because we have many references to mvpData
    // So I'll become crazy if I decide to update them all
    core_2d_exchange_fields_lv1(mvpData, mvpTemporalData, mvSettings.sizeX, mvSettings.sizeY);

    mvState.tCurrent += mvSettings.tStep;
    ++mvTemporalCurrentStep;
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
    return mvTemporalTotalSteps;
}

progress_unit_t Model::getCurrentStep()
{
    return mvTemporalCurrentStep;
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

void Model::initTemporalVariables()
{
    allocateTemporalShadowData();
    allocateTemporalDirectSumData();
    allocateTemporalReverseSumData();

    mvTemporalNu0   = 1.0e+13;
    mvTemporalEs    = 0;
    mvTemporalTheta = mvSettings.temperature * (PHYSICS_CONSTANT_BOLTZMAN / PHYSICS_CONSTANT_EV);

    mvTemporalPhiAA = -5e-20;
    mvTemporalPhiAB = -2e-20;
    mvTemporalPhiBB = -6e-20;

    mvTemporalTotalSteps    = floor((mvSettings.tEnd - mvSettings.tStart) / mvSettings.tStep);
    mvTemporalCurrentStep   = floor((mvState.tCurrent - mvSettings.tStart) / mvSettings.tStep);
}

void Model::cleanupTemporalVariables()
{
    if (mvpTemporalReverseSumData) {
        core_2d_deallocate_field(mvpTemporalReverseSumData, mvSettings.sizeX, mvSettings.sizeY);
        mvpTemporalReverseSumData = 0;
    }
    if (mvpTemporalDirectSumData) {
        core_2d_deallocate_field(mvpTemporalDirectSumData, mvSettings.sizeX, mvSettings.sizeY);
        mvpTemporalDirectSumData = 0;
    }
    if (mvpTemporalData) {
        core_2d_deallocate_field(mvpTemporalData, mvSettings.sizeX, mvSettings.sizeY);
        mvpTemporalData = 0;
    }
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
    return mvTemporalNu0 * exp(-(mvTemporalEs - getInteractionEnergy(a, b)) / mvTemporalTheta);
}

double Model::getInteractionEnergy(atom_reference_2d_t &a, atom_reference_2d_t &b)
{
    return mvTemporalPhiAA * getNeighboursDirectSum(a) + mvTemporalPhiAB * getNeighboursReverseSum(a) +
            mvTemporalPhiAB * getNeighboursDirectSum(b) + mvTemporalPhiBB * getNeighboursReverseSum(b);
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
    if (atom.field != mvpData) {
        throw std::logic_error("bad atom reference passed: wrong buffer address");
    }

    if (!mvpAtomNeighbourIteratorsLv1) {
        mvpAtomNeighbourIteratorsLv1 = allocateNeighbourIterators();
    }

    AtomNeighbourIterator *iterator = mvpAtomNeighbourIteratorsLv1[atom.y][atom.x];
    iterator->reset();

    return *iterator;
}

atom_value_t Model::getGridDataSum()
{
    atom_value_t sum = 0;

    AtomGridIterator &it = getAtomGridIterator();

    for (; !it.atEnd(); ++it) {
        sum += CORE_2D_RESOLVE_ATOM_REFERENCE(*it);
    }

    return sum;
}

atom_value_t Model::getSystemSum()
{
    return getGridDataSum();
}

void Model::describeIntoImplementation(std::ostream &output)
{
    output << "Simple 2D model" << std::endl;

    output << "  " << std::setw(24) << std::left << "Grid X-size:"
           << std::setw(16) << std::right << std::fixed << std::dec << (int) mvSettings.sizeX << std::endl;
    output << "  " << std::setw(24) << std::left << "Grid Y-size:"
           << std::setw(16) << std::right << std::fixed << std::dec << (int) mvSettings.sizeY << std::endl;

    output << "  " << std::setw(24) << std::left << "Temperature:"
           << std::setw(16) << std::right << std::fixed << std::dec << mvSettings.temperature << std::endl;
    output << "  " << std::setw(24) << std::left << "Start time:"
           << std::setw(16) << std::right << std::fixed << std::dec << mvSettings.tStart << std::endl;
    output << "  " << std::setw(24) << std::left << "End time:"
           << std::setw(16) << std::right << std::fixed << std::dec << mvSettings.tEnd << std::endl;
    output << "  " << std::setw(24) << std::left << "Time step:"
           << std::setw(16) << std::right << std::fixed << std::dec << mvSettings.tStep << std::endl;
    output << "  " << std::setw(24) << std::left << "Time current:"
           << std::setw(16) << std::right << std::fixed << std::dec << mvState.tCurrent << std::endl;

    output << "  " << std::setw(24) << std::left << "Total modelling steps:"
           << std::setw(16) << std::right << std::fixed << std::dec << getTotalSteps() << std::endl;
    output << "  " << std::setw(24) << std::left << "Current modelling step:"
           << std::setw(16) << std::right << std::fixed << std::dec << getCurrentStep() << std::endl;
    output << "  " << std::setw(24) << std::left << "Current percentage:"
           << std::setw(15) << std::right << std::fixed << std::dec << std::setprecision(2)
           << getProgressPercentage() << "%" << std::endl;
}

} // namespace Simple2D
