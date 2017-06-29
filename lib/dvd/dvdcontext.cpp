/* UML Class Diagram
@startuml

class DvDContext <<MediaSender>> {
+{abstract}open(string) : bool

-- slot --
+highlight(MenuButton)
+activate(MenuButton)
+menu()
+pause()
+resume()
}

@enduml
*/

#include "dvd/dvdcontext.h"

#include "dvd/MediaFrame.h"

// Std Libraries
#include <iostream>

// libDvDNav Includes
#include <dvdnav/dvdnav.h>
#include <dvdread/dvd_reader.h>

// Qt Includes
#include <QTimer>
#include <QCoreApplication>
#include <QSize>
#include <QRect>
#include <QList>

DvDContext::DvDContext(QObject* parent)
    : QObject(parent)
    , m_handle(0)
    , m_title("UNKNOWN")
    , m_loop(new QTimer)
    , m_mediaState(dvd::MediaNotAvailable)
    , m_flush(false)
{
    qRegisterMetaType<dvd::MediaFrame>("dvd::MediaFrame");
    qRegisterMetaType<dvd::StreamAction>("dvd::StreamAction");
    qRegisterMetaType<MenuButton>("MenuButton");
    qRegisterMetaType<QList<MenuButton> >("QList<MenuButton>");

    connect( m_loop, SIGNAL(timeout()), this, SLOT(loop()) );
    m_loop->setInterval(1);
}

DvDContext::~DvDContext()
{
    delete m_loop;
}

void DvDContext::setMediaState( dvd::MediaState state )
{
    if ( state != m_mediaState )
    {
        m_mediaState = state;
        emit mediaStateChanged(m_mediaState);
    }
}

bool DvDContext::open( QString const& device )
{
    // Emit the signal indicating the disk is loading
    setMediaState(dvd::MediaLoading);
    bool opresult(false);

    dvd_reader_t* reader( DVDOpen(qPrintable(device)) );
    if ( reader )
    {
        char volid[32];

        if ( DVDUDFVolumeInfo(reader,volid,32,0,0) == 0 )
            m_title = QString(volid);
        else if ( DVDISOVolumeInfo(reader,volid,32,0,0) == 0 )
            m_title = QString(volid);

        DVDClose(reader);
    }

    emit title( m_title );

    if ( m_handle )
    {
        dvdnav_close(m_handle);
        m_handle = 0;
    }

    if ( dvdnav_open(&m_handle, qPrintable(device)) == DVDNAV_STATUS_OK )
    {
        m_loop->start();
        opresult = true;
    }

    if ( opresult )
    {
        m_flush = true;
        resume();
    }
    else
        setMediaState(dvd::MediaNotAvailable);

    return opresult;
}

void DvDContext::pause()
{
    switch ( m_mediaState )
    {
    case dvd::MediaNotAvailable:
        break;
    case dvd::MediaLoading:
        break;
    case dvd::MediaIdle:
        break;
    case dvd::MediaReading:
        setMediaState(dvd::MediaIdle);
        QTimer::singleShot(0,m_loop,SLOT(stop()));
        break;
    }
}

void DvDContext::resume()
{
    switch ( m_mediaState )
    {
    case dvd::MediaNotAvailable:
        break;
    case dvd::MediaReading:
        break;
    case dvd::MediaLoading:
        // Intentional fallthrough
    case dvd::MediaIdle:
        setMediaState(dvd::MediaReading);
        QTimer::singleShot(0,m_loop,SLOT(start()));
        break;
    }
}

void DvDContext::loop()
{
    if ( !m_handle ) { return; }

    char buffer[DVD_VIDEO_LB_LEN];
    uint8_t* buffer_p(reinterpret_cast<uint8_t*>(buffer));

    int event, len;
    if ( dvdnav_get_next_block(m_handle, buffer_p, &event, &len) == DVDNAV_STATUS_ERR )
    {
        std::cerr << "Error: " << dvdnav_err_to_string(m_handle) << std::endl;
        dvdnav_close(m_handle);
        m_handle = 0;
    }
    else
    {
        switch(event)
        {
        case DVDNAV_BLOCK_OK:
        {
            // Convert the boolean to flush to the proper enumeration
            dvd::MediaFrame::Action action(dvd::MediaFrame::Append);
            if ( m_flush ){ action = dvd::MediaFrame::Flush; }

            // Reset stream action to default action
            m_flush = false;

            // Put the data into a QByteArray format
            QByteArray const data(buffer,len);

            // Convert the data into MediaFrames
            QList<dvd::MediaFrame> frms(dvd::MediaFrame::frames(data, action));

            // Transmit the frames
            foreach ( dvd::MediaFrame frm, frms )
                emit stream(frm);
        }
            break;
        case DVDNAV_NOP:
            break;
        case DVDNAV_STILL_FRAME:
            dvdnav_still_skip(m_handle);
            break;
        case DVDNAV_WAIT:
            dvdnav_wait_skip(m_handle);
            break;
        case DVDNAV_SPU_CLUT_CHANGE:
            break;
        case DVDNAV_SPU_STREAM_CHANGE:
            break;
        case DVDNAV_HIGHLIGHT:
            break;
        case DVDNAV_VTS_CHANGE:
        {
            uint32_t width(0), height(0);
            dvdnav_get_video_resolution(m_handle,&width,&height);
            QSizeF res(width,height);

            if ( res != m_resolution )
            {
                std::cout << "Resolution changed" << std::endl;
                m_resolution = res;
                emit resolution(res);
            }
        }
            break;
        case DVDNAV_CELL_CHANGE:
            break;
        case DVDNAV_NAV_PACKET:
        {
            // Get the nav_pci struct pointer
            pci_t* pci( dvdnav_get_current_nav_pci(m_handle) );
            Q_ASSERT( pci );

            bool btnupdate(false);

            int const btncnt(pci->hli.hl_gi.btn_ns);

            // The button list length is different than reported by the DVD,
            // clear the list to maintain sync
            if ( m_buttons.length() != btncnt )
            {
                btnupdate = true;
                m_buttons.clear();
            }

            for ( int i(0); i < btncnt; ++i )
            {
                // DVDRead Button structure representation
                btni_t const& btni(pci->hli.btnit[i]);

                // Pull the button's Size and Location
                QPoint const btnloc( btni.x_start, btni.y_start );
                QSize const btnsize( btni.x_end - btni.x_start,
                                     btni.y_end - btni.y_start );

                MenuButton button(i);
                button.setResolution( m_resolution );
                button.setButtonRect( QRectF(btnloc, btnsize) );

                if ( m_buttons.length() > i )
                {
                    MenuButton& other( m_buttons[i] );

                    // Update the current button if needed
                    if ( other.update(button) )
                        btnupdate = true;
                }
                else
                {
                    // Button was not contained, add the button
                    m_buttons << button;

                    // Flag the list as changed
                    btnupdate = true;
                }
            }

            // Emit the changed button list
            if ( btnupdate )
            { emit buttons(m_buttons); }

            dvdnav_get_current_nav_dsi(m_handle);
        }
            break;
        case DVDNAV_HOP_CHANNEL:
            // Set the next stream emit to trigger a buffer flush
            m_flush = true;
            break;
        case DVDNAV_STOP:
            break;
        }
    }
}

void DvDContext::highlight( MenuButton const& btn )
{
    int const index( btn.index() );
    if ( !btn.isNull() &&
         m_buttons.length() > index && m_buttons.at(index) == btn )
    {
        pci_t* pci(dvdnav_get_current_nav_pci(m_handle));

        if ( pci )
        {
            dvdnav_button_select(m_handle,pci,index+1);
        }
    }
}

void DvDContext::activate( MenuButton const& btn )
{
    std::cout << "Activate signal" << std::endl;
    if ( btn.isNull() ) return;

    int const index( btn.index() );
    if ( m_buttons.length() > index && m_buttons.at(index) == btn )
    {
        pci_t* pci(dvdnav_get_current_nav_pci(m_handle));

        if ( pci )
        {
            std::cout << "+++++++Activating " << index+1 << "+++++++" << std::endl;
            dvdnav_button_select_and_activate(m_handle,pci,index+1);
        }
    }
}
