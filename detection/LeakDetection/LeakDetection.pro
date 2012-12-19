#-------------------------------------------------
#
# Project created by QtCreator 2012-05-10T19:34:01
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = LeakDetection
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
    vtkKMeansClustering.h \
    vtkKMeansClustering.h
