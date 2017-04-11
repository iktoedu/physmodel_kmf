#ifndef CORE_MODEL_H
#define CORE_MODEL_H

#include "core/core_types.h"

class CoreModel
{
public:
    CoreModel();

    virtual bool isInitialized() = 0;
    virtual void think() = 0;
    virtual progress_unit_t getTotalSteps() = 0;
    virtual progress_unit_t getCurrentStep() = 0;
    virtual bool isModellingEnded() = 0;
};

#endif // CORE_MODEL_H
