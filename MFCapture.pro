#-------------------------------------------------
#
# Project created by QtCreator 2015-03-16T05:11:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MFCapture
TEMPLATE = app

LIBS += evr.lib
LIBS += mf.lib
LIBS += mfplat.lib
LIBS += Mfreadwrite.lib
LIBS += Mfuuid.lib
LIBS += Ole32.lib

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
