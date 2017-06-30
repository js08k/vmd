/*
@startuml

QObject <|-- MediaReceiver
MediaContext <|-- MediaReceiver
class MediaReceiver {
-tryPauseStream()
-tryResumeStream()
-bool m_streamPaused

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
