#-------------------------------------------------
#
# Project created by QtCreator 2016-09-08T14:59:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QMAKE_CXXFLAGS += -std=c++11

TARGET = SharpGame
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    defines.cpp

HEADERS  += mainwindow.h \
    defines.h \
    sharpgame.h

FORMS    += mainwindow.ui
