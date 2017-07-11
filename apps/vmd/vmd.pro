#-------------------------------------------------
#
# Project created by QtCreator 2017-05-20T12:07:06
#
#-------------------------------------------------

QT += core
QT += gui
QT += network
QT += multimedia
QT += multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = vmd
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
INCLUDEPATH += \
    /usr/local/protobuf/include \
    $$OUT_PWD/../../lib/gtqtbuild \
    ../../include

LIBS += \
    -L/usr/local/protobuf/lib \
    -lprotobuf \
    -L$$OUT_PWD/../../lib/gtqtbuild -lgtqt \
    -L../../lib/dvd -ldvd \
    -L../../lib/guiUtil -lguiUtil \
    -L/usr/local/libdvdcss/lib -ldvdcss \
    -L/usr/local/libdvdnav/lib -ldvdnav \
    -L/usr/local/libdvdread/lib -ldvdread

SOURCES += \
    main.cpp \
    vmd.cpp \
    netinspector.cpp \
    linkmanager.cpp \
    Vmd.cpp

HEADERS  += \
    vmd.h \
    netinspector.h \
    linkmanager.h \
    Vmd.h

FORMS    += \
    vmd.ui
