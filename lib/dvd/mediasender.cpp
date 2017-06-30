/*
@startuml

QObject <|-- MediaSender
MediaContext <|-- MediaSender
class MediaSender  {
-tryPauseStream()
-tryResumeStream()
-bool m_streamPaused

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
