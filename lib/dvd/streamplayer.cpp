#include "dvd/streamplayer.h"
#include "dvd/streambuffer.h"
#include "dvd/MediaFrame.h"

dvd::StreamPlayer::StreamPlayer(QObject *parent)
    : QMediaPlayer( parent, QMediaPlayer::VideoSurface )
    , m_buffer(new StreamBuffer(this))
{
    m_time.start();
    m_buffer->open(QIODevice::ReadWrite);
    connect( m_buffer, SIGNAL(pauseReadStream()), SIGNAL(pauseReadStream()) );
    connect( m_buffer, SIGNAL(resumeReadStream()), SIGNAL(resumeReadStream()) );
    connect( m_buffer, SIGNAL(ready()), this, SLOT(ready()) );
}

void dvd::StreamPlayer::stream(dvd::MediaFrame frame)
{
    switch ( frame.action() )
    {
    case gtqt::MediaFrame::DecryptFlush:
        frame.decrypt( QString() );
    case gtqt::MediaFrame::Flush:
        stop();
        m_buffer->stream(QByteArray(frame.data().c_str(),frame.data().length()), dvd::FlushStream );
        break;
    case gtqt::MediaFrame::DecryptAppend:
        frame.decrypt( QString() );
    case gtqt::MediaFrame::Append:
        m_buffer->stream(QByteArray(frame.data().c_str(),frame.data().length()), dvd::AppendStream );
        break;
    }
}

void dvd::StreamPlayer::ready()
{
    switch ( state() )
    {
    case QMediaPlayer::PlayingState:
        break;
    case QMediaPlayer::StoppedState:
        setMedia( QMediaContent(), m_buffer );
    case QMediaPlayer::PausedState:
        play();
        break;
    }
}
