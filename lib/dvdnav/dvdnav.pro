TARGET = dvdnav
TEMPLATE = lib

CONFIG -= qt
CONFIG += sharedLib

DEFINES += HAVE_CONFIG_H

SOURCES += \
    dvdnav.c \
    highlight.c \
    navigation.c \
    read_cache.c \
    searching.c \
    settings.c \
    vm/decoder.c \
    vm/getset.c \
    vm/play.c \
    vm/vm.c \
    vm/vmcmd.c \
    vm/vmget.c

HEADERS += \
    config.h \
    dvdnav_internal.h \
    read_cache.h \
    vm/decoder.h \
    vm/getset.h \
    vm/play.h \
    vm/vmcmd.h \
    vm/vm.h \
    ../../include/dvdnav/dvdnav_events.h \
    ../../include/dvdnav/dvdnav.h \
    ../../include/dvdnav/dvd_types.h

INCLUDEPATH += \
    ../../include
