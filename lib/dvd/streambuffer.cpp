#include "dvd/streambuffer.h"
#include <QtGlobal>
#include <QTimer>
#include <iostream>
#include <cmath>

StreamBuffer::StreamBuffer(QObject* parent)
    : QIODevice(parent)
    , m_minbuffered(1024*1024)
    , m_maxbuffered(m_minbuffered*2)
    , m_loop( new QTimer(this) )
{
    open( QIODevice::ReadWrite );
    connect( m_loop, SIGNAL(timeout()), this, SLOT(loop()) );
    m_loop->start(10);
}

StreamBuffer::StreamBuffer(QByteArray const& other, QObject* parent)
    : QIODevice(parent)
    , m_minbuffered(1024*1024)
    , m_maxbuffered(m_minbuffered*2)
    , m_data(other)
    , m_loop( new QTimer(this) )
{
    open( QIODevice::ReadWrite );
    connect( m_loop, SIGNAL(timeout()), this, SLOT(loop()) );
    m_loop->start(10);
}

StreamBuffer::~StreamBuffer()
{

}

void StreamBuffer::close()
{
    std::cout << "StreamBuffer::close()" << std::endl;
    setOpenMode( QIODevice::NotOpen );
}

bool StreamBuffer::open(OpenMode flags)
{
    std::cout << "StreamBuffer::open()" << std::endl;
    setOpenMode(flags);
    return true;
}

qint64 StreamBuffer::pos() const
{
//    std::cout << "StreamBuffer::pos()" << std::endl;
    return 0;
}

qint64 StreamBuffer::bytesAvailable() const
{
//    std::cout << "StreamBuffer::bytesAvailable()" << std::endl;
    return m_data.size();
}

bool StreamBuffer::seek(qint64 pos)
{
    QIODevice::seek(pos);
//    std::cout << "StreamBuffer::seek(" << pos << ")" << std::endl;
    return false;
}

qint64 StreamBuffer::size() const
{
//    std::cout << "StreamBuffer::size()" << std::endl;
    return m_data.size();
}

qint64 StreamBuffer::readData(char *data, qint64 maxSize)
{
    maxSize = qMin( maxSize, qint64(m_data.length()) );
    memcpy(data, m_data.data(), maxSize );
    m_data = m_data.remove(0, maxSize);
    return maxSize;
}

qint64 StreamBuffer::writeData(char const* data, qint64 size)
{
    m_data += QByteArray(data,size);
    return 0;
}

void StreamBuffer::stream(QByteArray const& data, dvd::StreamAction action)
{
    if ( !data.isEmpty() )
    {
        switch ( action )
        {
        case dvd::AppendStream:
            write(data);
            break;
        case dvd::FlushStream:
            m_data.clear();
            write(data);
            break;
        }

        bool isPlayable( size() > m_minbuffered );

        // Control the playback based on buffered data
        if ( !isPlayable && m_data.size() > m_minbuffered )
        {
            std::cout << "StreamBuffer: Resume playback" << std::endl;
            emit resumePlayback();
        }
        else if ( m_data.size() < m_minbuffered )
        {
            std::cout << "StreamBuffer: Pausing playback" << std::endl;
            emit pausePlayback();
        }
    }
}

void StreamBuffer::loop()
{
    if ( m_data.length() > m_maxbuffered )
        emit pauseReadStream();
    else if ( m_data.length() < (m_maxbuffered+m_minbuffered)/2 )
        emit resumeReadStream();
}
