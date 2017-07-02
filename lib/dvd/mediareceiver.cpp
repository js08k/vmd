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
