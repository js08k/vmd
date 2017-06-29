TEMPLATE = subdirs

SUBDIRS += \
    #gtqtbuild \
    #util \
    #dvd \
    dvdread \
    dvdnav \
    dvdcss \
    protobuf

dvd.depends += gtqtbuild
dvd.subdir = dvd

OTHER_FILES += \
    uml.pri
