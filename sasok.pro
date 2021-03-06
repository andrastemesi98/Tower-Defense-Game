######################################################################
# Automatically generated by qmake (3.1) Mon Apr 29 16:38:52 2019
######################################################################

TEMPLATE = app
TARGET = sasok
INCLUDEPATH += .

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += network

CONFIG += C++11

# The following define makes your compiler warn you if you use any
# feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
HEADERS += Include/base.h \
           Include/basics.h \
           Include/field.h \
           Include/game.h \
           Include/path.h \
           Include/player.h \
           Include/tower.h \
           Include/unit.h \
           Include/view.h
SOURCES += Source/base.cpp \
           Source/field.cpp \
           Source/game.cpp \
           Source/main.cpp \
           Source/path.cpp \
           Source/player.cpp \
           Source/tower.cpp \
           Source/unit.cpp \
           Source/view.cpp
