#-------------------------------------------------
#
# Project created by QtCreator 2011-12-05T09:30:09
#
#-------------------------------------------------

QT       += core gui

TARGET = QT-VTK-Viewer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

LIBS    += -L/usr/lib/vtk-5.8 -lQVTK\
           -L/usr/lib/vtk-5.8 -lvtkRendering\
           -L/usr/lib/vtk-5.8 -lvtkCommon\
           -L/usr/lib/vtk-5.8 -lvtkIO\
           -L/usr/lib/vtk-5.8 -lvtkFiltering

INCLUDEPATH += /usr/include/vtk-5.8

FORMS    += mainwindow.ui
