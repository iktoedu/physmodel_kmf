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

    while (!model->isModellingEnded()) {
        model->think();

        clock_t currentTime = clock();

        double timeTotal = float((currentTime - beginTime) / CLOCKS_PER_SEC) / model->getCurrentStep() * totalSteps;

        cout << "\b\b\b\b\b\b\b\b\b\b\b\b" << setw(12) << timeTotal;

//        cout << "\b\b\b\b\b\b\b\b\b\b\b\b" << setw(12) << model->getCurrentStep();
    }

    cout << endl;

    ofstream file;
    file.open("test.xyz");
    simple_2d_dump_model_to_xyz((Simple2D::Model *) model, file);
    file.close();

    return 0;
}
