/*
@startuml

enum MediaState {
    MediaNotAvailable
    MediaLoading
    MediaIdle
    MediaReading
}

enum Action {
    ActionAppend
    ActionFlush
    ActionDecryptAppend
    ActionDecryptFlush
}

enum StreamControl {
    SendInfo
    PauseStream
    ResumeStream
    PausePlayback
    ResumePlayback
}

enum MediaType {
    TypeUnknown
    TypeDVD
    TypeMPEG
    TypePeer
}

@enduml
*/
#include "dvd/dvd.h"

#include "dvd/MediaFrame.h"
#include "dvd/menubutton.h"

#include <QVector>
#include <QList>

void dvd::registerMetaTypes()
{
    qRegisterMetaType<dvd::MediaFrame>("dvd::MediaFrame");
    qRegisterMetaType<dvd::MenuButton>("dvd::MenuButton");
    qRegisterMetaType<QList<dvd::MenuButton>>("QVector<dvd::MenuButton>");

    // Not actually in use
    qRegisterMetaType<dvd::Action>("dvd::Action");
    qRegisterMetaType<QVector<dvd::MenuButton>>("QVector<dvd::MenuButton>");
}
