/*
@startuml

MediaSender <|-- MPEGContext
class MPEGContext <<MediaSender>> {
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

#include <QTimer>
#include <QCoreApplication>

#include <iostream>

MPEGContext::MPEGContext(QObject* parent)
    : QObject(parent)
    , m_loop(new QTimer(this))
    , m_buffer( 2048, '\0' )
{
    connect( m_loop, SIGNAL(timeout()), this, SLOT(loop()) );
    m_loop->setInterval(0);
}

MPEGContext::~MPEGContext()
{

}

bool MPEGContext::open( QString const& device )
{
    bool opresult(false);

    m_file.close();

    m_file.setFileName(device);
    if ( m_file.open(QIODevice::ReadOnly) )
    {
        m_loop->start();
        opresult = true;
    }

    return opresult;
}

void MPEGContext::pauseRead()
{
    if ( m_loop->isActive() )
    {
        std::cout << "MPEGContext: Pausing read" << std::endl;
        m_loop->stop();
    }
}

void MPEGContext::resumeRead()
{
    if ( !m_loop->isActive() )
    {
        std::cout << "MPEGContext: Resuming read" << std::endl;
        m_loop->start();
    }
}

void MPEGContext::loop()
{
    qint64 length( m_file.read(m_buffer.data(), m_buffer.length()) );
    if ( length == 0 )
    {
        m_file.seek(0);
        return;
    }

    if ( length == m_buffer.length() )
    {
        emit media( m_buffer );
    }
    else if ( length > 0 )
    {
        emit media( m_buffer.mid(0, length) );
    }
}
