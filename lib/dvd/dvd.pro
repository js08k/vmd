#-------------------------------------------------
#
# Project created by QtCreator 2017-06-11T13:49:16
#
#-------------------------------------------------

QT += gui multimedia multimediawidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dvd
TEMPLATE = lib

DEFINES += DVD_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += \
    ../../include \
    /usr/local/protobuf/include \
    $$OUT_PWD/../../lib/gtqtbuild \
    /usr/local/libdvdcss/include \
    /usr/local/libdvdnav/include \
    /usr/local/libdvdread/include

SOURCES += \
    dvdcontext.cpp \
    mpegcontext.cpp \
    streambuffer.cpp \
    VideoWidget.cpp \
    menubutton.cpp \
    streamplayer.cpp \
    MediaFrame.cpp \
    mediainput.cpp \
    mediacontext.cpp \
    networkcontext.cpp \
    streamdelay.cpp

HEADERS += \
    ../../include/dvd/dvd_global.h \
    ../../include/dvd/dvdcontext.h \
    ../../include/dvd/mpegcontext.h \
    ../../include/dvd/streambuffer.h \
    ../../include/dvd/VideoWidget.h \
    ../../include/dvd/menubutton.h \
    ../../include/dvd/streamplayer.h \
    ../../include/dvd/MediaFrame.h \
    ../../include/dvd/mediainput.h \
    ../../include/dvd/mediacontext.h \
    ../../include/dvd/networkcontext.h \
    ../../include/dvd/streamdelay.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

LIBS += \
    -L/usr/local/libdvdcss/lib -ldvdcss \
    -L/usr/local/libdvdnav/lib -ldvdnav \
    -L/usr/local/libdvdread/lib -ldvdread

OTHER_FILES += \
    classes.uml


