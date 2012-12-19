#-------------------------------------------------
#
# Project created by QtCreator 2012-08-09T12:56:18
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = ModelCombiner
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

VTKLIBDIR = /usr/lib/vtk-5.8
VTKINCLUDEDIR = /usr/include/vtk-5.8

LIBS    += -L${VTKLIBDIR} -lQVTK\
           -L${VTKLIBDIR} -lvtkRendering\
           -L${VTKLIBDIR} -lvtkCommon\
           -L${VTKLIBDIR} -lvtkIO\
           -L${VTKLIBDIR} -lvtkFiltering\
           -L${VTKLIBDIR} -lvtkGraphics\
           -L${VTKLIBDIR} -lvtkImaging

INCLUDEPATH += ${VTKINCLUDEDIR}


SOURCES += main.cpp
