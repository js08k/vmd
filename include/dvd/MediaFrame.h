#ifndef MEDIAFRAME_H
#define MEDIAFRAME_H

#include "message.h"
#include <QList>

namespace dvd { class MediaFrame; }
class QString;

class dvd::MediaFrame
        : public gtqt::MediaFrame
{
public:
    enum Action
    {
        Append = gtqt::MediaFrame::Append,
        Flush = gtqt::MediaFrame::Flush
    };

    MediaFrame();
    MediaFrame( gtqt::MediaFrame const& other );
    static QList<dvd::MediaFrame> frames( QByteArray, Action actn = Append );
    void encrypt( QString const& key );
    void decrypt( QString const& key );
};

#endif // MEDIAFRAME_H
