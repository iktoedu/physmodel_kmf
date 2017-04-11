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

    void init(double, double, double);

    bool isInitialized();

    void think();

    model_settigns_t getModelSettings();
    model_state_t getModelState();

    atom_value_t **getDataPointer();

    progress_unit_t getTotalSteps();
    progress_unit_t getCurrentStep();

    bool isModellingEnded();

private:

    bool mvIsInitialized;

    model_settigns_t mvSettings;
    model_state_t mvState;

    atom_value_t **mvpData = 0;
    atom_value_t **mvpShadowData = 0;

    void allocateData();
    void allocateShadowData();
};

} // namespace Simple2D

#endif // SIMPLE_2D_MODEL_H
