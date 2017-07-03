/*
@startuml

QObject <|-- MediaContext
interface MediaContext {
+MediaContext(QObject* = 0)
+{abstract}~MediaContext()
+{abstract}open(QString) : bool = 0
+state() const : MediaState
#setMediaState(MediaState)
-MediaState m_state

-- signals --
+stream(MediaFrame)
+title(QString)
+mediaStateChange(MediaState)
+buttons(QVector<MenuButton>)

-- slots --
+{abstract}pauseStream() = 0
+{abstract}resumeStream() = 0
+{abstract}menu() = 0
+{abstract}highlight(MenuButton) = 0
+{abstract}activate(MenuButton) = 0
}

@enduml
*/
#include "dvd/mediacontext.h"

dvd::MediaContext::MediaContext(QObject* parent)
    : QObject(parent)
    , m_state(dvd::NotAvailable)
{
    qRegisterMetaType<QVector<dvd::MenuButton>>("QVector<dvd::MenuButton>");
}

dvd::MediaContext::~MediaContext()
{

}

void dvd::MediaContext::setMediaState( dvd::MediaState const& state )
{
    if ( state != m_state )
    {
        m_state = state;
        emit mediaStateChange(m_state);
    }
}
