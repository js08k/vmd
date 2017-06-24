#ifndef DVD_GLOBAL_H
#define DVD_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DVD_LIBRARY)
#  define DVDSHARED_EXPORT Q_DECL_EXPORT
#else
#  define DVDSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // DVD_GLOBAL_H
