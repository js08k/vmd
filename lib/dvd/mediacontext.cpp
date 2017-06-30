/*
@startuml

interface MediaContext {
+{abstract}open(QString) : bool = 0

-- signals --
+stream(MediaFrame) = 0
+{abstract}pauseStream() = 0
+{abstract}resumeStream() = 0
}

@enduml
*/
#include "dvd/mediacontext.h"
