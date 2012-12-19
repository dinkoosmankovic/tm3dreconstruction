#-------------------------------------------------
#
# Project created by QtCreator 2011-12-13T15:22:21
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = FalseScans
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


SOURCES += main.cpp \
    vtkKMeansClustering.cpp

HEADERS += \
    vtkKMeansClustering.h
