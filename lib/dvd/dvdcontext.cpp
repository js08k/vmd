#include "dvd/dvdcontext.h"

// Std Libraries
#include <iostream>

// libDvDNav Includes
#include <dvdnav/dvdnav.h>

// Qt Includes
#include <QTimer>
#include <QCoreApplication>
#include <QSize>
#include <QRect>
#include <QList>

DvDContext::DvDContext(QObject* parent)
    : QObject(parent)
    , m_handle(0)
    , m_loop(new QTimer(this))
    , m_mediaState(dvd::MediaNotAvailable)
    , m_nextStreamAction(dvd::AppendStream)
{
    connect( m_loop, SIGNAL(timeout()), this, SLOT(loop()) );
    m_loop->setInterval(0);

    QTimer::singleShot( 20000, this, SLOT(showResolution()) );
}

void DvDContext::showResolution()
{
    resolution();
    QTimer::singleShot( 20000, this, SLOT(showResolution()) );
}

DvDContext::~DvDContext()
{

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
        resume();
    }
    else
    {
        setMediaState(dvd::MediaNotAvailable);
    }

    return opresult;
}

QSizeF DvDContext::resolution() const
{
    if ( !m_handle ) { return QSize(); }

    uint32_t width(0), height(0);
    dvdnav_get_video_resolution(m_handle,&width,&height);

    return QSizeF(width,height);
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
        m_loop->stop();
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
        m_loop->start();
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
            resolution();

            // Emit the media stream
            emit stream( QByteArray(buffer,len), m_nextStreamAction );

            // Reset stream action to default action
            m_nextStreamAction = dvd::AppendStream;

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
            std::cout << "++++++Highlight++++++" << std::endl;
            break;
        case DVDNAV_VTS_CHANGE:
            break;
        case DVDNAV_CELL_CHANGE:
            break;
        case DVDNAV_NAV_PACKET:
        {
            pci_t* pci(dvdnav_get_current_nav_pci(m_handle));

            if ( !pci )
            {
                std::cerr << "Error: " << dvdnav_err_to_string(m_handle) << std::endl;
            }
            else
            {
                bool btnupdate(false);
                QSizeF const res( resolution() );

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
                    button.setResolution( res );
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

                if ( btncnt > 0 )
                {
                    std::cout << "Waiting for a button selection..." << std::endl;
//                    m_loop->stop();
                }
            }
        }
            break;
        case DVDNAV_HOP_CHANNEL:
            // Set the next stream emit to trigger a buffer flush
            m_nextStreamAction = dvd::FlushStream;
            break;
        case DVDNAV_STOP:
            std::cout << "DVDNAV_STOP state" << std::endl;
//            QCoreApplication::exit(0);
            break;
        }
    }
}

void DvDContext::navToTitle1()
{
    if ( m_handle &&
         dvdnav_title_play(m_handle, int32_t(1)) != DVDNAV_STATUS_OK )
    {
        std::cout << "Nav to Title1 failed" << std::endl;
        QTimer::singleShot(1,this,SLOT(navToTitle1()));
    }
    else
    {
        std::cout << "Nav to Title1 succeeded" << std::endl;
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
            dvdnav_button_select(m_handle,pci,index);
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
            std::cout << "+++++++Activating " << index << "+++++++" << std::endl;
            dvdnav_button_select_and_activate(m_handle,pci,index);
        }
    }
}
