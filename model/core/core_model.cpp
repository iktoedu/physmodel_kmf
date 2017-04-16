#include "core_model.h"
#include <iomanip>

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

void CoreModel::describeInto(std::ostream &output)
{
    std::ios::fmtflags flags = output.flags();

    describeIntoImplementation(output);

    output.flags(flags);
}

void CoreModel::describeIntoImplementation(std::ostream &output)
{
    output << "Abstract core model" << std::endl;

    output << "  " << std::setw(24) << std::left << "Total modelling steps:"
           << std::setw(16) << std::right << std::fixed << std::dec << getTotalSteps() << std::endl;
    output << "  " << std::setw(24) << std::left << "Current modelling step:"
           << std::setw(16) << std::right << std::fixed << std::dec << getCurrentStep() << std::endl;
    output << "  " << std::setw(24) << std::left << "Current percentage:"
           << std::setw(15) << std::right << std::fixed << std::dec << std::setprecision(2)
           << getProgressPercentage() << "%" << std::endl;
}
