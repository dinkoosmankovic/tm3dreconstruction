#-------------------------------------------------
#
# Project created by QtCreator 2012-06-11T17:24:58
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = FalseScans_FCM
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

INCLUDEPATH += /usr/include/vtk-5.8 \
               /usr/include/eigen3



SOURCES += main.cpp \
    #vtkFuzzyCMeans.cpp \
    vtkFuzzyCMeansE3.cpp

HEADERS += \
    #vtkFuzzyCMeans.h \
    vtkFuzzyCMeansE3.h
