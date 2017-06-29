TEMPLATE = subdirs

SUBDIRS += \
    gtqtbuild \
    util \
    dvd

dvd.depends += gtqtbuild
dvd.subdir = dvd

OTHER_FILES += \
    uml.pri
