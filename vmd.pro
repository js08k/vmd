# Required packages: libprotoc-dev, protobuf-compiler
#
# To update gtqt:
#   rm -rf gtqt
#   svn export https://bently/svn/gtqt/trunk/gtqt
#   (svn export ../gtqt/gtqt)
#

TEMPLATE = subdirs

SUBDIRS += \
    gtqt \
    apps \
    lib \
    #installer

lib.depends = gtqt
lib.subdir = lib

apps.depends = gtqt lib
apps.subdir = apps

installer.depends = apps
installer.subdir = installer

OTHER_FILES += \
    README.md \
    Makefile

HEADERS += \
    include/guiUtil/videowidget2.h

SOURCES += \
    lib/guiUtil/videowidget2.cpp
