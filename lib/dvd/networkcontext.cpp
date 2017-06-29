/*
@startuml

class NetworkContext <<MediaReceiver>>{
+{abstract}open(QString) : bool

-- slots --
+highlight(MenuButton)
+activate(MenuButton)
+menu()
+pause()
+resume()
}

@enduml
*/
#include "dvd/networkcontext.h"
