/*
@startuml

MediaContext <|-- MediaReceiver
class MediaReceiver {
-tryPauseStream()
-tryResumeStream()
-bool m_streamPaused

-- signals --
+send(StreamControl)

-- slots --
+receive(MediaFrame)
+highlight(MenuButton)
+activate(MenuButton)
+menu()
+pause()
+resume()
}

@enduml
*/
#include "dvd/mediareceiver.h"
#include <iostream>

dvd::MediaReceiver::MediaReceiver(QObject* parent)
    : dvd::MediaContext(parent)
{

}

void dvd::MediaReceiver::receive( dvd::MediaFrame const& )
{

}

void dvd::MediaReceiver::pauseStream()
{

    std::cout << "dvd::MediaReceiver::pauseStream()" << std::endl;
}

void dvd::MediaReceiver::resumeStream()
{
    std::cout << "dvd::MediaReceiver::resumeStream()" << std::endl;
}

void dvd::MediaReceiver::menu()
{
    std::cout << "dvd::MediaReceiver::menu()" << std::endl;
}

void dvd::MediaReceiver::highlight(dvd::MenuButton const&)
{
    std::cout << "dvd::MediaReceiver::highlight()" << std::endl;
}

void dvd::MediaReceiver::activate(dvd::MenuButton const&)
{
    std::cout << "dvd::MediaReceiver::activate()" << std::endl;
}
