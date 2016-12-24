#-------------------------------------------------
#
# Project created by QtCreator 2016-12-01T19:10:48
#
#-------------------------------------------------

QT       += core gui serialport widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = picoSerialPlotter
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settingdialog.cpp \
    qcustomplot.cpp \
    plotter.cpp

HEADERS  += mainwindow.h \
    settingdialog.h \
    qcustomplot.h \
    plotter.h

FORMS    += mainwindow.ui \
    plotter.ui

RESOURCES += \
    icons.qrc
