#-------------------------------------------------
#
# Project created by QtCreator 2016-09-19T15:43:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Goldorak
TEMPLATE = app

LIBS += -L/usr/local/lib -lopencv_core -lopencv_highgui
unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
}
SOURCES += main.cpp\
        mainwindow.cpp \
    widget.cpp

HEADERS  += mainwindow.h \
    widget.h
