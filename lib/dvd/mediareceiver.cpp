/*
@startuml

MediaContext <|-- MediaReceiver
class MediaReceiver <<MediaContext>> {
-- signals --
{field}connected to MediaSender
+send(StreamControl)

-- slots --
{field}connected to MediaSender
+receive(MediaFrame)
}

@enduml
*/
#include "dvd/mediareceiver.h"
