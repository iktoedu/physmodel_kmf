#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <cmath>
#include <csignal>
#include "core/core_model.h"
#include "simple_2d/simple_2d_util.h"

using namespace std;

#define SIZE_X   40
#define SIZE_Y   40

struct {
    bool isSigIntHasBeenTriggered = false;
} globalState;

void handleSigInt(int signum)
{
    globalState.isSigIntHasBeenTriggered = true;
}

int main(int argc, char *argv[])
{
    signal(SIGINT, handleSigInt);

    CoreModel *model = simple_2d_init_new_model(SIZE_X, SIZE_Y, 10, 1.0e-10);

    model->describeInto(cout);

    const progress_unit_t totalSteps = model->getTotalSteps();

    const clock_t beginTime = clock();
    clock_t currentTime;
    double timeTotal, timeSpent;
    progress_unit_t currentStep;

    atom_value_t initialSystemSum = model->getSystemSum();
    uint8_t systemSumCheckCounter = 0;

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
        ++systemSumCheckCounter;
        if (systemSumCheckCounter == 100) {
            if (fabs(initialSystemSum - model->getSystemSum()) > 1e-1) {
                cout << endl << endl << "[EMERG] SYSTEM SUM HAS BEEN CHANGED!!" << endl;
                cout << "Initial: " << initialSystemSum << endl;
                cout << "Current: " << model->getSystemSum() << endl;
                exit(1);
            }
            systemSumCheckCounter = 0;
        }

        if (globalState.isSigIntHasBeenTriggered) {
            cout << endl << endl << "Caught SIGNINT" << endl;
            break;
        }
    }

    cout << endl;

    ofstream file;
    file.open("test.xyz");
    simple_2d_dump_model_to_xyz((Simple2D::Model *) model, file);
    file.close();

    return 0;
}
