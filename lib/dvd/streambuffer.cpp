/*
@startuml

class StreamBuffer <<QIODevice>>{
'Required for implementation of QIODevice'
+{abstract}isSequential( ) const final : bool
+{abstract}bytesAvailable( ) const final : qint64
+{abstract}size() const final : qint64
-{abstract}readData(char*, qint64) final : qint64
-{abstract}writeData(char const*, qint64) final

-- private --
-int const m_minbuffered
-int const m_maxbuffered
-QByteArray m_data
}

@enduml
*/

#include "dvd/streambuffer.h"
#include "dvd/MediaFrame.h"
#include <QtGlobal>
#include <QTimer>

dvd::StreamBuffer::StreamBuffer(QObject* parent)
    : QIODevice(parent)
    , m_minbuffered(1024*1024*4)
    , m_maxbuffered(m_minbuffered*2)
    , m_ready(false)
{
    open( QIODevice::ReadWrite );
}

dvd::StreamBuffer::StreamBuffer(QByteArray const& other, QObject* parent)
    : QIODevice(parent)
    , m_minbuffered(1024*1024)
    , m_maxbuffered(m_minbuffered*2)
    , m_data(other)
    , m_ready(false)
{
    open( QIODevice::ReadWrite );
}

dvd::StreamBuffer::~StreamBuffer()
{

}

void dvd::StreamBuffer::close()
{
    setOpenMode( QIODevice::NotOpen );
}

bool dvd::StreamBuffer::open(OpenMode flags)
{
    setOpenMode(flags);
    return true;
}

qint64 dvd::StreamBuffer::pos() const
{
    return 0;
}

qint64 dvd::StreamBuffer::bytesAvailable() const
{
    return m_data.size();
}

bool dvd::StreamBuffer::seek(qint64 pos)
{
    QIODevice::seek(pos);
    return false;
}

qint64 dvd::StreamBuffer::size() const
{
    return m_data.size();
}

qint64 dvd::StreamBuffer::readData(char *data, qint64 maxSize)
{
    maxSize = qMin( maxSize, qint64(m_data.length()) );
    memcpy(data, m_data.data(), maxSize );
    m_data = m_data.remove(0, maxSize);

    if ( m_data.length() < (m_maxbuffered+m_minbuffered)/2 )
    {
        emit resumeReadStream();
    }
    else if ( m_data.isEmpty() )
    {
        m_ready = false;
    }

    return maxSize;
}

qint64 dvd::StreamBuffer::writeData(char const* data, qint64 size)
{
    m_data += QByteArray(data,size);
    return 0;
}

void dvd::StreamBuffer::stream(dvd::MediaFrame frame)
{
    if ( frame.data().length() > 0 )
    {
        switch ( dvd::Action(frame) )
        {
        case dvd::DecryptAppend:
            frame.decrypt("todo");
        case dvd::Append:
            write(frame.data().c_str(), frame.data().length());
            break;
        case dvd::DecryptTruncate:
            frame.decrypt("todo");
        case dvd::Truncate:
            m_ready = false;
            m_data.clear();
            write(frame.data().c_str(), frame.data().length());
            break;
        }

        // Control the reading (input stream)
        if ( m_data.length() > m_maxbuffered )
        {
            emit pauseReadStream();
        }
        else if ( size() > m_minbuffered && !m_ready )
        {
            m_ready = true;
            emit ready();
        }
    }
}
