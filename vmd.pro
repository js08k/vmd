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
    lib

lib.depends = gtqt
lib.subdir = lib

apps.depends = gtqt lib
apps.subdir = apps
