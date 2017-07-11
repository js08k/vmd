#include "Vmd.h"

#include "guiUtil/VideoWidget.h"
#include "dvd/mediainput.h"
#include "dvd/streamplayer.h"
#include "dvd/dvdcontext.h"

#include <QSettings>

#include <iostream>

gui::VMD::VMD( QObject* parent )
    : QObject(parent)
    , m_addrexp("^((?:\\d{1,3}[.]){3}\\d{1,3}):(\\d+)$")
    , m_widget(0)
    , m_player(0)
    , m_context(0)
{
}

void gui::VMD::setVideoWidget( gui::VideoWidget* widget )
{
    if ( m_widget )
    {
        // todo place code to handle this if this is how it is to be used
        Q_ASSERT(false);
    }
    else
    {
        m_widget = widget;

        // Restore settings
        QSettings settings;
        settings.beginGroup("gui");
        m_widget->setPeerAddress(settings.value("PeerAddr").toString());
        m_widget->setHostAddress(settings.value("HostAddr").toString());

        connect( m_widget, SIGNAL(clickedConnect()), SLOT(toggledConnect()) );
        connect( m_widget, SIGNAL(peerAddressChanged(QString)), SLOT(peerAddressChanged(QString)) );
        connect( m_widget, SIGNAL(hostAddressChanged(QString)), SLOT(hostAddressChanged(QString)) );
        connect( m_widget, SIGNAL(loadMedia(QString)), this, SLOT(loadMedia(QString)) );
    }
}

void gui::VMD::toggledConnect()
{

}

void gui::VMD::loadMedia( QString const& media )
{
    std::cout << "Load media: " << media.toStdString() << " " << m_widget << std::endl;
    if ( !m_widget ) { return; }
    std::cout << "Now loading the media..." << std::endl;
    dvd::MediaInput input;
    if ( input.initialize(media) )
    {
        if ( m_context )
        { delete m_context; }

        m_context = input.create(this);
        if ( m_context )
        {
//            m_context->moveToThread( m_mediaThread );

//            m_link->transmit(gtqt::DataPackage<gtqt::MediaInfo>(m_mediainfo.msg));

            if ( m_player )
            { delete m_player; }

            m_player = new dvd::StreamPlayer(this);
            m_player->setVideoOutput(m_widget);

            connect( m_context, SIGNAL(resolution(QSizeF)),
                     this, SLOT(resolution(QSizeF)) );

            connect( m_player, SIGNAL(pauseReadStream()),
                     m_context, SLOT(pauseStream()) );

            connect( m_player, SIGNAL(resumeReadStream()),
                     m_context, SLOT(resumeStream()) );

            connect( m_context, SIGNAL(stream(dvd::MediaFrame)),
                     m_player, SLOT(stream(dvd::MediaFrame)) );

//            connect( m_context, SIGNAL(stream(dvd::MediaFrame)),
//                     m_link, SLOT(transmit(dvd::MediaFrame)) );

            connect( m_widget, SIGNAL(highlight(dvd::MenuButton)),
                     m_context, SLOT(highlight(dvd::MenuButton)) );

            connect( m_widget, SIGNAL(activate(dvd::MenuButton)),
                     m_context, SLOT(activate(dvd::MenuButton)) );

            connect( m_context, SIGNAL(buttons(QList<dvd::MenuButton>)),
                     m_widget, SLOT(buttons(QList<dvd::MenuButton>)) );

            connect( m_context, SIGNAL(title(QString)),
                     this, SLOT(setTitle(QString)) );

            m_context->open( input.path() );
        }
    }
}
#include <QHostAddress>
void gui::VMD::peerAddressChanged( QString const& addr )
{
    std::cout << "addr: " << addr.toStdString() << std::endl;
    QRegExp exp( m_addrexp );
    if ( exp.indexIn(addr) >= 0 )
    {
        QSettings settings;
        settings.beginGroup("gui");
        settings.setValue("PeerAddr",addr);

        std::cout << "cap(1): " << exp.cap(1).toStdString() << std::endl;
        QHostAddress const host( exp.cap(1) );
        std::cout << "cap(2): " << exp.cap(2).toStdString() << std::endl;
        quint16 const port( exp.cap(2).toInt() );
        Q_UNUSED(host)
        Q_UNUSED(port)
    }
}

void gui::VMD::hostAddressChanged( QString const& addr )
{
    std::cout << "addr: " << addr.toStdString() << std::endl;
    QRegExp exp( m_addrexp );
    if ( exp.indexIn(addr) >= 0 )
    {
        QSettings settings;
        settings.beginGroup("gui");
        settings.setValue("HostAddr",addr);

        std::cout << "cap(1): " << exp.cap(1).toStdString() << std::endl;
        QHostAddress const host( exp.cap(1) );
        std::cout << "cap(2): " << exp.cap(2).toStdString() << std::endl;
        quint16 const port( exp.cap(2).toInt() );
        Q_UNUSED(host)
        Q_UNUSED(port)
    }
}
