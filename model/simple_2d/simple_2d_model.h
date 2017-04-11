#ifndef SIMPLE_2D_MODEL_H
#define SIMPLE_2D_MODEL_H

#include "core/core_types.h"
#include "core/core_model.h"

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
} model_settigns_t;

class Model : public CoreModel
{
public:
    Model(position_value_t, position_value_t);
    Model(model_settigns_t, model_state_t, atom_value_t **);
    ~Model();

    void think();

    model_settigns_t getModelSettings();
    model_state_t getModelState();

    atom_value_t **getDataPointer();

    progress_unit_t getTotalSteps();
    progress_unit_t getCurrentStep();

    bool isModellingEnded();

private:
    model_settigns_t settings;
    model_state_t state;

    atom_value_t **data = 0;
    atom_value_t **shadowData = 0;

    void allocateData();
    void allocateShadowData();
};

} // namespace Simple2D

#endif // SIMPLE_2D_MODEL_H
