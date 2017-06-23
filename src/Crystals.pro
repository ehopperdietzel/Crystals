#-------------------------------------------------
#
# Project created by QtCreator 2017-06-17T02:42:59
#
#-------------------------------------------------

QT       += core gui x11extras network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Crystals
TEMPLATE = app
LIBS += -lX11

SOURCES += \
        main.cpp \
    events.cpp \
    appwindow.cpp \
    windowmanager.cpp

HEADERS += \
    events.h \
    includes.h \
    appwindow.h \
    windowmanager.h \
    huincha.h \
    coreclient.h \
    windowbuttons.h \
    topbar.h

RESOURCES += \
    resources.qrc
