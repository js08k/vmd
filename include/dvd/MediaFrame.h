#pragma once

#include "dvd/dvd.h"
#include "message.h"
#include <QList>

class QString;

namespace dvd { class MediaFrame; }
namespace dvd
{
    // Factory function
    QVector<MediaFrame> frames(QByteArray, Action actn = Append );
}

class dvd::MediaFrame
        : public gtqt::MediaFrame
{
public:
    MediaFrame();
    MediaFrame( gtqt::MediaFrame const& other );
    void encrypt( QString const& key );
    void decrypt( QString const& key );
    explicit operator dvd::Action() const;
    inline explicit operator QByteArray() const;
};

dvd::MediaFrame::operator QByteArray() const
{
    return QByteArray( data().c_str(), data().length() );
}
