/*
@startuml

class MediaContext <<QObject>>{
+{abstract}open(QString) : bool

'Allow for pausing/resuming the reading stream'
#{abstract}pauseStream()
#{abstract}resumeStream()

-- signals --
+stream(MediaFrame)
+title(QString)
+resolution(QSizeF)
+mediaStateChanged(MediaState)

'-- slots --'
'+{abstract}highlight(MenuButton)'
'+{abstract}activate(MenuButton)'
'+{abstract}menu()'
'+pause()'
'+resume()'
}

@enduml
*/
#include "dvd/mediacontext.h"
