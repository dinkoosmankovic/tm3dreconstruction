#-------------------------------------------------
#
# Project created by QtCreator 2012-12-10T21:59:37
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = ModelExporter
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
           -L${VTKLIBDIR} -lvtkImaging\
           -L${VTKLIBDIR} -lvtkHybrid\

INCLUDEPATH += /usr/include/vtk-5.8


SOURCES += main.cpp
