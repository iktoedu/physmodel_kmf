#include "core_model.h"

CoreModel::CoreModel()
{
}

void CoreModel::preThink()
{
}

void CoreModel::postThink()
{
}

atom_value_t CoreModel::getSystemSum()
{
    return 0;
}

float CoreModel::getProgressPercentage()
{
    return (float) getCurrentStep() / (float) getTotalSteps();
}
