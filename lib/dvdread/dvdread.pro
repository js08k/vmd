TARGET = dvdread
TEMPLATE = lib

CONFIG -= qt
CONFIG += sharedLib

SOURCES += \
    bitreader.c \
    dvd_input.c \
    dvd_reader.c \
    dvd_udf.c \
    ifo_print.c \
    ifo_read.c \
    md5.c \
    nav_print.c \
    nav_read.c

HEADERS += \
    config.h \
    bswap.h \
    dvd_input.h \
    dvdread_internal.h \
    md5.h \
    ../../include/dvdread/bitreader.h \
    ../../include/dvdread/dvd_reader.h \
    ../../include/dvdread/dvd_udf.h \
    ../../include/dvdread/ifo_print.h \
    ../../include/dvdread/ifo_read.h \
    ../../include/dvdread/ifo_types.h \
    ../../include/dvdread/nav_print.h \
    ../../include/dvdread/nav_read.h \
    ../../include/dvdread/nav_types.h

INCLUDEPATH += \
    ../../include
