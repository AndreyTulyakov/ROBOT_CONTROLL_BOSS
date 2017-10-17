#-------------------------------------------------
#
# Project created by QtCreator 2017-10-15T13:34:21
#
#-------------------------------------------------

QT += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ROBOT_CONTROLL_BOSS
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    key_press_eater.cpp

HEADERS  += mainwindow.h \
    key_press_eater.h

FORMS    += mainwindow.ui
