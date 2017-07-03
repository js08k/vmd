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

dvd::NetworkContext::NetworkContext(QObject* parent)
    : dvd::MediaReceiver(parent)
{

}

void dvd::NetworkContext::open( QString const& device )
{
    std::cout << "dvd::NetworkContext::open("
              << device.toStdString() << ")" << std::endl;
}
