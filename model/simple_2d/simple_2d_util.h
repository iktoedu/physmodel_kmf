#ifndef SIMPLE_2D_UTIL_H
#define SIMPLE_2D_UTIL_H

#include <fstream>
#include "simple_2d/simple_2d_model.h"

Simple2D::Model *simple_2d_init_new_model(position_value_t, position_value_t, double, double);
void simple_2d_dump_model_to_xyz(Simple2D::Model *, std::ofstream &);

#endif // SIMPLE_2D_UTIL_H
