TARGET = dvdcss
TEMPLATE = lib

CONFIG -= qt
CONFIG += sharedLib

SOURCES += \
    css.c \
    device.c \
    error.c \
    ioctl.c \
    libdvdcss.c

HEADERS += \
    config.h \
    common.h \
    css.h \
    csstables.h \
    device.h \
    ioctl.h \
    libdvdcss.h \
    ../../include/dvdcss/dvdcss.h \
    ../../include/dvdcss/version.h

INCLUDEPATH += \
    ../../include
