#include "simple_2d/simple_2d_util.h"
#include "core_2d/core_2d_util.h"

Simple2D::Model *simple_2d_init_new_model(position_value_t sizeX, position_value_t sizeY, double tEnd, double tStep)
{
    Simple2D::Model *model = new Simple2D::Model(sizeX, sizeY);

    srand(time(NULL));
    core_2d_generate_field_values(model->getDataPointer(), sizeX, sizeY, [](position_value_t, position_value_t) {
        return (rand() % 2 == 1) ? .75 : .25;
    });

    model->init(0, tEnd, tStep);

    return model;
}
