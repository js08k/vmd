#-------------------------------------------------
#
# Project created by QtCreator 2017-06-11T13:47:55
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DvDPlayer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        DvDPlayer.cpp \
    MediaScreen.cpp \
    qmediaplayerspy.cpp

HEADERS += \
        DvDPlayer.h \
    MediaScreen.h \
    qmediaplayerspy.h

FORMS += \
        DvDPlayer.ui

INCLUDEPATH += \
    ../../include \
    /usr/local/libdvdcss/include \
    /usr/local/libdvdnav/include \
    /usr/local/libdvdread/include \
    /usr/local/ffmpeg/include

LIBS += \
    -L../../lib/dvd -ldvd \
    -L/usr/local/libdvdcss/lib -ldvdcss \
    -L/usr/local/libdvdnav/lib -ldvdnav \
    -L/usr/local/libdvdread/lib -ldvdread \
    -L/usr/local/ffmpeg/lib -lavcodec -lavutil -lswresample -ldl -lpthread
