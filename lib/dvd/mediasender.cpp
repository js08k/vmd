/*
@startuml

MediaContext <|-- MediaSender
class MediaSender  {
-tryPauseStream()
-tryResumeStream()
-bool m_streamPaused

-- signals --
+send(MediaFrame)

-- slots --
+receive(StreamControl)
}

@enduml
*/
#include "dvd/mediasender.h"

#include <iostream>

dvd::MediaSender::MediaSender( QObject* parent )
    : dvd::MediaContext(parent)
    , m_streamPaused(false)
{

}

void dvd::MediaSender::receive( gtqt::StreamRequest const& )
{
    std::cout << "MediaSender::receive(gtqt::StreamRequest)" << std::endl;
}

void dvd::MediaSender::tryPauseStream()
{
    if ( !m_streamPaused )
    {
        pauseStream();
        m_streamPaused = true;
    }
}

void dvd::MediaSender::tryResumeStream()
{
    if ( m_streamPaused )
    {
        resumeStream();
        m_streamPaused = false;
    }
}
