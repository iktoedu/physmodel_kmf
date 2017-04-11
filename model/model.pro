TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    simple_2d/simple_2d_atom_neighbour_iterator.cpp \
    core_2d/core_2d_util.cpp \
    simple_2d/simple_2d_atom_grid_iterator.cpp \
    core/core_model.cpp \
    simple_2d/simple_2d_model.cpp \
    simple_2d/simple_2d_util.cpp

HEADERS += \
    core/core_types.h \
    core_2d/core_2d_atom_reference.h \
    simple_2d/simple_2d_atom_neighbour_iterator.h \
    core_2d/core_2d_util.h \
    simple_2d/simple_2d_atom_grid_iterator.h \
    core/core_model.h \
    simple_2d/simple_2d_model.h \
    simple_2d/simple_2d_util.h
