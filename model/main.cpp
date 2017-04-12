#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include "core/core_model.h"
#include "simple_2d/simple_2d_util.h"

using namespace std;

#define SIZE_X   40
#define SIZE_Y   40

int main(int argc, char *argv[])
{

    CoreModel *model = simple_2d_init_new_model(SIZE_X, SIZE_Y, 10, 1.0e-10);

    const progress_unit_t totalSteps = model->getTotalSteps();

    cout << "Steps count: " << totalSteps << endl;

    const clock_t beginTime = clock();
    clock_t currentTime;
    double timeTotal, timeSpent;
    progress_unit_t currentStep;

    while (!model->isModellingEnded()) {
        model->think();

        currentTime = clock();
        currentStep = model->getCurrentStep();

        timeTotal = float((currentTime - beginTime) / CLOCKS_PER_SEC) / currentStep * totalSteps;
        timeSpent = float((currentTime - beginTime) / CLOCKS_PER_SEC);

        cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b"
                << setw(12) << currentStep
                << setw(12) << timeTotal
                << setw(12) << timeSpent;
    }

    cout << endl;

    ofstream file;
    file.open("test.xyz");
    simple_2d_dump_model_to_xyz((Simple2D::Model *) model, file);
    file.close();

    return 0;
}
