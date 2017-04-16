#ifndef CORE_MODEL_H
#define CORE_MODEL_H

#include <iostream>
#include "core/core_types.h"

class CoreModel
{
public:
    CoreModel();

    virtual bool isInitialized() = 0;
    virtual void think() = 0;
    virtual progress_unit_t getTotalSteps() = 0;
    virtual progress_unit_t getCurrentStep() = 0;
    virtual float getProgressPercentage();
    virtual bool isModellingEnded() = 0;

    virtual atom_value_t getSystemSum();

    void describeInto(std::ostream &);

protected:
    virtual void preThink();
    virtual void postThink();
    virtual void describeIntoImplementation(std::ostream &);
};

#endif // CORE_MODEL_H
