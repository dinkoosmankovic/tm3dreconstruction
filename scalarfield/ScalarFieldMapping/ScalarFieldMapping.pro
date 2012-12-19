#-------------------------------------------------
#
# Project created by QtCreator 2011-10-21T11:08:47
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = ScalarFieldMapping
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

VTKLIBDIR = /usr/lib/vtk-5.8

LIBS    += -L${VTKLIBDIR} -lQVTK\
           -L${VTKLIBDIR} -lvtkRendering\
           -L${VTKLIBDIR} -lvtkCommon\
           -L${VTKLIBDIR} -lvtkIO\
           -L${VTKLIBDIR} -lvtkFiltering\
           -L${VTKLIBDIR} -lvtkGraphics\
           -L${VTKLIBDIR} -lvtkImaging

INCLUDEPATH += /usr/include/vtk-5.8

SOURCES += main.cpp
