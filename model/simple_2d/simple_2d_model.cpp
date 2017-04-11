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

    mvIsInitialized     = true;
}

bool Model::isInitialized()
{
    return mvIsInitialized;
}

void Model::think()
{
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
    return (mvState.tCurrent < mvSettings.tEnd);
}

void Model::allocateData()
{
    mvpData = core_2d_allocate_field(mvSettings.sizeX, mvSettings.sizeY);
}

void Model::allocateShadowData()
{
    mvpShadowData = core_2d_allocate_field(mvSettings.sizeX, mvSettings.sizeY);
}

} // namespace Simple2D
