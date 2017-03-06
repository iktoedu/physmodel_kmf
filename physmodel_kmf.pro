TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    atom_neighbour_iterator.cpp

HEADERS += \
    atom_reference.h \
    basic_types.h \
    atom_neighbour_iterator.h
