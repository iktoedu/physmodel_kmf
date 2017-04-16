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
    bool isSigIntHasBeenTriggered   = false;
    bool isSigUsr1HasBeenTriggered  = false;
} globalState;

void handleSigInt(int)
{
    globalState.isSigIntHasBeenTriggered = true;
}

void handleSigUsr1(int)
{
    globalState.isSigUsr1HasBeenTriggered = true;
}

int main(int argc, char *argv[])
{
    signal(SIGINT, handleSigInt);
    signal(SIGUSR1, handleSigUsr1);

    CoreModel *model = simple_2d_init_new_model(SIZE_X, SIZE_Y, 10, 1.0e-10);

    model->describeInto(cout);

    const progress_unit_t totalSteps = model->getTotalSteps() - model->getCurrentStep();
    progress_unit_t currentStep = 0;

    const clock_t beginTime = clock();


    atom_value_t initialSystemSum = model->getSystemSum();
    uint8_t systemSumCheckCounter = 0;

    while (!model->isModellingEnded()) {
        model->think();
        ++currentStep;

        if (globalState.isSigUsr1HasBeenTriggered) {
            model->describeInto(cout);

            clock_t currentTime = clock();
            double timeTotal = float((currentTime - beginTime) / CLOCKS_PER_SEC) / currentStep * totalSteps;
            double timeSpent = float((currentTime - beginTime) / CLOCKS_PER_SEC);

            cout << setw(12) << timeTotal
                 << setw(12) << timeSpent
                 << endl;

            globalState.isSigUsr1HasBeenTriggered = false;
        }

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
