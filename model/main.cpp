#include <iostream>
#include "core/core_model.h"
#include "simple_2d/simple_2d_util.h"

using namespace std;

#define SIZE_X   5
#define SIZE_Y   10

int main(int argc, char *argv[])
{

    CoreModel *model = simple_2d_init_new_model(SIZE_X, SIZE_Y, 10, 1.0e-10);

    return 0;
}
