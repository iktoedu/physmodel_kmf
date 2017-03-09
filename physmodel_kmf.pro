TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    simple_2d/simple_2d_atom_neighbour_iterator.cpp

HEADERS += \
    core/core_types.h \
    core_2d/core_2d_atom_reference.h \
    simple_2d/simple_2d_atom_neighbour_iterator.h
