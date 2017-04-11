#include "simple_2d_model.h"
#include "core_2d/core_2d_util.h"

#include <cmath>

namespace Simple2D {

Model::Model(position_value_t sizeX, position_value_t sizeY)
{
    settings.sizeX = sizeX;
    settings.sizeY = sizeY;
    allocateData();
    allocateShadowData();
}

Model::Model(model_settigns_t settings, model_state_t state, atom_value_t **data)
    : settings(settings), state(state), data(data)
{
    allocateShadowData();
}

Model::~Model()
{
    if (shadowData) {
        core_2d_deallocate_field(shadowData, settings.sizeX, settings.sizeY);
    }
    core_2d_deallocate_field(data, settings.sizeX, settings.sizeY);
}

model_settigns_t Model::getModelSettings()
{
    return settings;
}

model_state_t Model::getModelState()
{
    return state;
}

atom_value_t **Model::getDataPointer()
{
    return data;
}

void Model::think()
{
    state.tCurrent += settings.tStep;
}

progress_unit_t Model::getTotalSteps()
{
    return floor((settings.tEnd - settings.tStart) / settings.tStep);
}

progress_unit_t Model::getCurrentStep()
{
    return floor((state.tCurrent - settings.tStart) / settings.tStep);
}

bool Model::isModellingEnded()
{
    return (state.tCurrent < settings.tEnd);
}

void Model::allocateData()
{
    data = core_2d_allocate_field(settings.sizeX, settings.sizeY);
}

void Model::allocateShadowData()
{
    shadowData = core_2d_allocate_field(settings.sizeX, settings.sizeY);
}

} // namespace Simple2D
