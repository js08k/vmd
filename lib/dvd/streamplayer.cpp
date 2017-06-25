#include "dvd/streamplayer.h"
#include "dvd/streambuffer.h"
#include <iostream>

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

void dvd::StreamPlayer::stream(QByteArray const& data, dvd::StreamAction action)
{
    switch ( action )
    {
    case dvd::AppendStream:
        break;
    case dvd::FlushStream:
        stop();
        break;
    }

    m_buffer->stream( data, action );
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
