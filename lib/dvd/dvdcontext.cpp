/* UML Class Diagram
@startuml

MediaSender <|-- DvDContext
class DvDContext {
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

dvd::DvDContext::DvDContext(QObject* parent)
    : MediaSender(parent)
    , m_handle(0)
    , m_title("UNKNOWN")
    , m_loop(new QTimer)
    , m_flush(false)
{
    qRegisterMetaType<dvd::MediaFrame>("dvd::MediaFrame");
    qRegisterMetaType<dvd::Action>("dvd::Action");
    qRegisterMetaType<dvd::MenuButton>("dvd::MenuButton");
    qRegisterMetaType<QList<dvd::MenuButton> >("QList<dvd::MenuButton>");

    connect( m_loop, SIGNAL(timeout()), this, SLOT(loop()) );
    m_loop->setInterval(1);
}

dvd::DvDContext::~DvDContext()
{
    delete m_loop;
}

void dvd::DvDContext::open( QString const& device )
{
    // Set the state of the DvDContext, loading new media
    setMediaState(dvd::Loading);

    // Query the DvD title info (using libdvdread since libdvdnav always
    // returns null results)
    if ( dvd_reader_t* handle = DVDOpen(qPrintable(device)) )
    {
        // Default dvd title to "UNKNOWN"
        char vol_id[32] = "UNKNOWN";

        // First try UDF, then try ISO to get title
        if ( DVDUDFVolumeInfo(handle,vol_id,32,0,0) == 0 ) { }
        else if ( DVDISOVolumeInfo(handle,vol_id,32,0,0) == 0 ) { }

        // Set the dvd title
        m_title = QString(vol_id);

        // Close the reader
        DVDClose(handle);
    }

    emit title( m_title );

    if ( m_handle )
    {
        dvdnav_close(m_handle);
        m_handle = 0;
    }

    if ( dvdnav_open(&m_handle, qPrintable(device)) == DVDNAV_STATUS_OK )
    {
        setReadStart();
        m_loop->start();
    }
    else
    {
        setMediaState(dvd::NotAvailable);
    }
}

void dvd::DvDContext::pauseStream()
{
    switch ( state() )
    {
    case dvd::NotAvailable:
        break;
    case dvd::Loading:
        break;
    case dvd::Idle:
        break;
    case dvd::Reading:
        setMediaState(dvd::Idle);
        QTimer::singleShot(0,m_loop,SLOT(stop()));
        break;
    }
}

void dvd::DvDContext::resumeStream()
{
    switch ( state() )
    {
    case dvd::NotAvailable:
        break;
    case dvd::Reading:
        break;
    case dvd::Loading:
        // Intentional fallthrough
        break;
    case dvd::Idle:
        setMediaState(dvd::Reading);
        QTimer::singleShot(0,m_loop,SLOT(start()));
        break;
    }
}

void dvd::DvDContext::setReadStart()
{
    // Navigate the DvD to the Menu
    if ( DVDNAV_STATUS_ERR == dvdnav_menu_call(m_handle, DVD_MENU_Root) )
        QTimer::singleShot( 0, this, SLOT(setReadStart()) );
}

void dvd::DvDContext::loop()
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
            if ( state() != dvd::Loading )
            {
                // Block streaming during the loading state

                // Convert the boolean to flush to the proper enumeration
                dvd::Action action(dvd::Append);
                if ( m_flush ){ action = dvd::Truncate; }

                // Reset stream action to default action
                m_flush = false;

                // Put the data into a QByteArray format
                QByteArray const data(buffer,len);

                // Convert the data into MediaFrames
                QVector<dvd::MediaFrame> frms(dvd::frames(data, action));

                // Transmit the frames
                for ( dvd::MediaFrame frm : frms )
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
            if ( state() == MediaState::Loading )
            {
                // End of the open function, menu has been loaded
                setMediaState( MediaState::Reading );
            }
            // Set the next stream emit to trigger a buffer flush
            m_flush = true;
            break;
        case DVDNAV_STOP:
            break;
        }
    }
}

void dvd::DvDContext::menu()
{
    std::cout << "todo: Implement dvd::DVDContext::menu()" << std::endl;
}

void dvd::DvDContext::highlight(MenuButton const& btn )
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

void dvd::DvDContext::activate(MenuButton const& btn )
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
