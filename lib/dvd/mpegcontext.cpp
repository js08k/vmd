/*
@startuml

MediaSender <|-- MPEGContext
class MPEGContext {
+{abstract}open(QString) : bool

-- slots --
+pause()
+resume()
'-highlight(MenuButton)'
'-activate(MenuButton)'
'-menu()'
}

@enduml
*/
#include "dvd/mpegcontext.h"
#include "dvd/MediaFrame.h"

#include <QTimer>
#include <QCoreApplication>

#include <iostream>

dvd::MPEGContext::MPEGContext(QObject* parent)
    : MediaSender(parent)
    , m_loop(new QTimer(this))
    , m_buffer( 1024, '\0' )
{
    connect( m_loop, SIGNAL(timeout()), this, SLOT(loop()) );
    m_loop->setInterval(0);
}

dvd::MPEGContext::~MPEGContext()
{

}

void dvd::MPEGContext::open( QString const& device )
{
    m_file.close();

    m_file.setFileName(device);
    if ( m_file.open(QIODevice::ReadOnly) )
    {
        std::cout << "Opened " << device.toStdString() << std::endl;
        m_loop->start();
    }
    else
    {
        std::cout << "Failed to open " << device.toStdString() << std::endl;
    }
}

void dvd::MPEGContext::pauseStream()
{
    if ( m_loop->isActive() )
    {
        std::cout << "MPEGContext: Pausing read" << std::endl;
        m_loop->stop();
    }
}

void dvd::MPEGContext::resumeStream()
{
    if ( !m_loop->isActive() )
    {
        std::cout << "MPEGContext: Resuming read" << std::endl;
        m_loop->start();
    }
}

void dvd::MPEGContext::loop()
{
    qint64 length( m_file.read(m_buffer.data(), m_buffer.length()) );
    if ( length == 0 )
    {
        m_file.seek(0);
        return;
    }

    // TODO: This fails if we read less than the buffer length (aka last kB)
    if ( length == m_buffer.length() )
    {
        dvd::Action action(dvd::Append);

        QVector<dvd::MediaFrame> frms(dvd::frames(m_buffer,action));

        for ( dvd::MediaFrame frm : frms )
            emit stream(frm);
    }
}
