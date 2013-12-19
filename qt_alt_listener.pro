#-------------------------------------------------
#
# Project created by QtCreator 2013-12-19T08:18:28
#
#-------------------------------------------------

QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt_alt_listener
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    socklisten.cpp \
    threadmonitor.cpp

HEADERS  += mainwindow.h \
    socklisten.h \
    threadmonitor.h

FORMS    += mainwindow.ui
