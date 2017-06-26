#ifndef STREAMPLAYER_H
#define STREAMPLAYER_H

#include <QMediaPlayer>
#include "dvd/dvdcontext.h"
#include <QTime>
#include "MediaFrame.h"

namespace dvd { class StreamPlayer; }
namespace dvd { class MediaFrame; }

//namespace dvd { class StreamBuffer; }
class StreamBuffer;

class dvd::StreamPlayer
        : public QMediaPlayer
{
    Q_OBJECT
public:
    StreamPlayer( QObject* parent = 0 );

signals:
    void pauseReadStream() const;
    void resumeReadStream() const;

public slots:
    void stream(dvd::MediaFrame frame);

private slots:
    void ready();

private:
    StreamBuffer* m_buffer;
    QTime m_time;
};

#endif // STREAMPLAYER_H
