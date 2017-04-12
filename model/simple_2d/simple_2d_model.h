#ifndef SIMPLE_2D_MODEL_H
#define SIMPLE_2D_MODEL_H

#include "core/core_types.h"
#include "core/core_model.h"
#include "core_2d/core_2d_atom_reference.h"
#include "simple_2d/simple_2d_atom_grid_iterator.h"
#include "simple_2d/simple_2d_atom_neighbour_iterator.h"


namespace Simple2D {

typedef struct {
    double tCurrent;
} model_state_t;

typedef struct {
    position_value_t sizeX;
    position_value_t sizeY;
    double tStart;
    double tEnd;
    double tStep;
    double temperature;
} model_settigns_t;

class Model : public CoreModel
{
public:
    Model(position_value_t, position_value_t);
    Model(model_settigns_t, model_state_t, atom_value_t **);
    ~Model();

    void init(double, double, double, double);

    bool isInitialized();

    void think();

    model_settigns_t getModelSettings();
    model_state_t getModelState();

    atom_value_t **getDataPointer();

    progress_unit_t getTotalSteps();
    progress_unit_t getCurrentStep();

    bool isModellingEnded();

protected:
    void preThink();
    void postThink();

private:

    bool mvIsInitialized;

    model_settigns_t mvSettings;
    model_state_t mvState;

    atom_value_t **mvpData = 0;

    atom_value_t **mvpTemporalData = 0;
    atom_value_t **mvpTemporalDirectSumData = 0;
    atom_value_t **mvpTemporalReverseSumData = 0;

    double mvTemporalNu0;
    double mvTemporalEs;
    double mvTemporalTheta;
    double mvTemporalPhiAA;
    double mvTemporalPhiAB;
    double mvTemporalPhiBB;

    void allocateData();
    void allocateTemporalShadowData();
    void allocateTemporalDirectSumData();
    void allocateTemporalReverseSumData();

    void initTemporalVariables();
    void cleanupTemporalVariables();

    atom_value_t getAtomDelta(atom_reference_2d_t &);
    double getAtomExProb(atom_reference_2d_t &, atom_reference_2d_t &);
    double getInteractionEnergy(atom_reference_2d_t &, atom_reference_2d_t &);

    atom_value_t getNeighboursDirectSum(atom_reference_2d_t &);
    atom_value_t getNeighboursReverseSum(atom_reference_2d_t &);

    // Let's create atom grid iterator only once
    // For other iterations - re-use existing
    AtomGridIterator *mvpAtomGridIterator = 0;
    AtomGridIterator &getAtomGridIterator();

    AtomNeighbourIterator ***mvpAtomNeighbourIteratorsLv1 = 0;
    AtomNeighbourIterator ***mvpAtomNeighbourIteratorsLv2 = 0;

    AtomNeighbourIterator ***allocateNeighbourIterators();
    void deallocateNeighbourIterators(AtomNeighbourIterator ***);
    AtomNeighbourIterator &getAtomNeighbourLv1Iterator(const atom_reference_2d_t &);
    AtomNeighbourIterator &getAtomNeighbourLv2Iterator(const atom_reference_2d_t &);
};

} // namespace Simple2D

#endif // SIMPLE_2D_MODEL_H
