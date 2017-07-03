#include "dvd/mediainput.h"
#include <iostream>

// libDvDRead
#include "dvdread/dvd_reader.h"

// Qt Includes
#include <QRegExp>
#include <QFileInfo>

dvd::MediaInput::MediaInput()
    : m_type(TypeUnknown)
    , m_name("UNKNOWN")
{

}

bool dvd::MediaInput::initialize( QString const& name )
{
    QRegExp exp( "^(dvd|file|peer)://(\\S+)$" );
    exp.indexIn( name );

    if ( exp.cap(1) == "dvd" )
    {
        m_path = exp.cap(2);
        std::cout << "dvd: " << exp.cap(0).toStdString() << std::endl;
        return tryDVD(exp.cap(2));
    }
    else if ( exp.cap(1) == "file" )
    {
        m_path = exp.cap(2);
        std::cout << "file: " << exp.cap(0).toStdString() << std::endl;
        return tryFile(exp.cap(2));
    }
    else if ( exp.cap(1) == "peer" )
    {
        m_path = exp.cap(2);
        std::cout << "peer: " << exp.cap(0).toStdString() << std::endl;
        return tryPeer(exp.cap(2));
    }

    return false;
}
#include "dvd/mediacontext.h"
#include "dvd/dvdcontext.h"
#include "dvd/mpegcontext.h"
#include "dvd/networkcontext.h"
dvd::MediaContext* dvd::MediaInput::create( QObject* parent ) const
{
    switch ( m_type )
    {
    case dvd::TypeDVD:
        std::cout << "Creating a dvd::DVDContext" << std::endl;
        return new dvd::DvDContext(parent);
        break;
    case dvd::TypeMPEG:
        std::cout << "Creating a dvd::MPEGContext" << std::endl;
        return new dvd::MPEGContext(parent);
        break;
    case dvd::TypePeer:
        std::cout << "Creating a dvd::NetworkContext" << std::endl;
        return new dvd::NetworkContext(parent);
        break;
    case dvd::TypeUnknown:
        std::cout << "Could not determine the context type" << std::endl;
        break;
    }

    return nullptr;
}

bool dvd::MediaInput::tryDVD( QString const& dev )
{
    // Query the DvD title info (using libdvdread)
    if ( dvd_reader_t* handle = DVDOpen(qPrintable(dev)) )
    {
        bool isOk(false);

        // Default dvd title to "UNKNOWN
        char vol_id[32] = "UNKNOWN";

        // Try UDF, then try ISO
        if ( DVDUDFVolumeInfo(handle,vol_id,32,0,0) == 0 ||
             DVDISOVolumeInfo(handle,vol_id,32,0,0) == 0 )
        {
            m_type = dvd::TypeDVD;
            isOk = true;
        }

        m_name = QString(vol_id);

        // Close the reader
        DVDClose(handle);

        return isOk;
    }

    return false;
}

bool dvd::MediaInput::tryFile( QString const& dev )
{
    QFileInfo filename( dev );
    bool const isOk( filename.exists() && filename.isReadable() );

    if ( isOk )
    {
        m_type = dvd::TypeMPEG;
        m_name = filename.baseName();
    }

    return isOk;
}

bool dvd::MediaInput::tryPeer( QString const& dev )
{
    QRegExp exp( "^((?:\\d{1,3}\\.){3}\\d{1,3}):(\\d+)/(\\S+)$" );

    if ( exp.indexIn(dev) >= 0 )
    {
        QString address( exp.cap(1) );
        quint16 port( exp.cap(2).toUShort() );

        Q_UNUSED(address);
        Q_UNUSED(port);

        m_name = exp.cap(3);

        std::cout << exp.cap(1).toStdString() << std::endl;
        std::cout << exp.cap(2).toStdString() << std::endl;
        std::cout << exp.cap(3).toStdString() << std::endl;
        m_type = dvd::TypePeer;

        return true;
    }

    return false;
}

QString dvd::MediaInput::name() const
{
    return m_name;
}

QString dvd::MediaInput::path() const
{
    return m_path;
}

