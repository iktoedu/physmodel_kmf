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

void simple_2d_dump_model_to_xyz(Simple2D::Model *model, std::ofstream &output)
{
    Simple2D::model_settigns_t modelSettings = model->getModelSettings();
    uint64_t size = modelSettings.sizeX * modelSettings.sizeY;
    atom_value_t **data = model->getDataPointer();

    output << size << std::endl << std::endl;

    for (position_value_t x = 0; x < modelSettings.sizeX; ++x) {
        for (position_value_t y = 0; y < modelSettings.sizeY; ++y) {
            output << (int) x << "\t" << (int) y << "\t" << data[y][x] << std::endl;
        }
    }
}
