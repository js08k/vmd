QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = player
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    player.cpp

HEADERS += \
    player.h

FORMS    += \
    player.ui

LIBS += -lVLCQtCore -lVLCQtWidgets
