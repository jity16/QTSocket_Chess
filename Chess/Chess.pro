#-------------------------------------------------
#
# Project created by QtCreator 2018-09-04T20:25:04
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Chess
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    loginwidget.cpp \
    gamewidget.cpp \
    game.cpp \
    boardwidget.cpp

HEADERS  += mainwindow.h \
    loginwidget.h \
    gamewidget.h \
    game.h \
    boardwidget.h

FORMS    += mainwindow.ui \
    loginwidget.ui \
    gamewidget.ui \
    boardwidget.ui

RESOURCES += \
    node.qrc

DISTFILES += \
    node/rma.png \
    bgm/operate.wav
