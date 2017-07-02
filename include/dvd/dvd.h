#pragma once

#include <QtCore/qglobal.h>

#if defined(DVD_LIBRARY)
#  define DVDSHARED_EXPORT Q_DECL_EXPORT
#else
#  define DVDSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace dvd
{
    void registerMetaTypes();

    // Forward class declarations
    class MenuButton;
    class MediaFrame;
    class DvDContext;
    class DvDInterface;
    class StreamInterface;
    class MediaContext;
    class MediaSender;
    class MPEGContext;
    class StreamBuffer;

    // Enum type declarations
    enum MediaState
    {
        NotAvailable,
        Loading,
        Idle,
        Reading
    };

    enum Action
    {
        Append,
        Truncate,
        DecryptAppend,
        DecryptTruncate
    };

    enum Control
    {
        SendInfo,
        PauseStream,
        ResumeStream,
        PausePlayback,
        ResumePlayback
    };

    enum MediaType
    {
        TypeUnknown,
        TypeDVD,
        TypeMPEG,
        TypePeer
    };
}
