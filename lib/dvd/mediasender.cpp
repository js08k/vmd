/*
@startuml

MediaContext <|-- MediaSender
class MediaSender <<MediaContext>> {

-- signals --
{field}connected to MediaReceiver
+send(MediaFrame)

-- slots --
{field}connected to MediaReceiver
+receive(StreamControl)
}

@enduml
*/
#include "dvd/mediasender.h"
