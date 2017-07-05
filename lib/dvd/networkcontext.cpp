/*
@startuml

MediaReceiver <|-- NetworkContext
class NetworkContext {
+{abstract}open(QString) : bool
}

@enduml
*/
#include "dvd/networkcontext.h"
#include <iostream>
#include "dvd/MediaFrame.h"

dvd::NetworkContext::NetworkContext(QObject* parent)
    : dvd::MediaReceiver(parent)
{
    connect( parent, SIGNAL(receive(gtqt::DataPackage<gtqt::MediaInfo>)),
             this, SLOT(receive(gtqt::DataPackage<gtqt::MediaInfo>)) );
    connect( parent, SIGNAL(receive(gtqt::DataPackage<gtqt::MediaFrame>)),
             this, SLOT(receive(gtqt::DataPackage<gtqt::MediaFrame>)) );
}

void dvd::NetworkContext::open( QString const& device )
{
    std::cout << "dvd::NetworkContext::open("
              << device.toStdString() << ")" << std::endl;
}

void dvd::NetworkContext::receive( gtqt::DataPackage<gtqt::MediaInfo> const& msg )
{
    std::cout << "Received MediaInfo" << std::endl;
    if ( msg->has_height() && msg->has_width() )
        emit resolution( QSizeF(msg->width(),msg->height()) );
}

void dvd::NetworkContext::receive( gtqt::DataPackage<gtqt::MediaFrame> const& msg )
{
    std::cout << "Received MediaFrame" << std::endl;
    dvd::MediaFrame frame(*msg.constData());
    dvd::MediaReceiver::receive( frame );
    emit stream( frame );
}
