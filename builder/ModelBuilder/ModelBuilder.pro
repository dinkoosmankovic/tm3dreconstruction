#-------------------------------------------------
#
# Project created by QtCreator 2011-02-14T11:13:35
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = ModelBuilder
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
