/*
@startuml

MediaReceiver <|-- NetworkContext
class NetworkContext {
+{abstract}open(QString) : bool
}

@enduml
*/
#include "dvd/networkcontext.h"
