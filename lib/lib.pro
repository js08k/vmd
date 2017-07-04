TEMPLATE = subdirs

SUBDIRS += \
    gtqtbuild \
    util \
    dvd \
    guiUtil

dvd.depends += gtqtbuild
dvd.subdir = dvd

OTHER_FILES += \
    uml.pri
