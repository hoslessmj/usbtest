#-------------------------------------------------
#
# Project created by QtCreator 2015-05-12T19:34:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = front_end
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    hcamer.cpp \
    hftdi.cpp \
    husb.cpp \
    husbdeviceoperator.cpp

HEADERS  += widget.h \
    hcamer.h \
    hftdi.h \
    hTools.h \
    husb.h \
    husbdeviceoperator.h

FORMS    += widget.ui

OTHER_FILES += \
    front_end.pro.user
