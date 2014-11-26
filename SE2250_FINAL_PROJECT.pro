#-------------------------------------------------
#
# Project created by QtCreator 2014-11-04T15:16:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SE2250_FINAL_PROJECT
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    level.cpp \
    gamemanager.cpp \
    user.cpp \
    lawn.cpp \
    plant.cpp \
    zombie.cpp

HEADERS  += mainwindow.h \
    ../user.h \
    level.h \
    gamemanager.h \
    user.h \
    lawn.h \
    plant.h \
    zombie.h

FORMS    += mainwindow.ui

RESOURCES += \
    pvz.qrc
