#include "dvd/mediainput.h"
#include <iostream>

// libDvDRead
#include "dvdread/dvd_reader.h"

// Qt Includes
#include <QRegExp>
#include <QFileInfo>

dvd::MediaInput::MediaInput( QString const& name )
    : m_name(name)
    , m_type(TypeDVD)
{
    QRegExp exp( "^(dvd|file)://(\\S+)$" );
    exp.indexIn( name );

    if ( exp.captureCount() == 2 )
    {
        if ( exp.cap(1) == "dvd" )
            m_type = TypeDVD;
        else if ( exp.cap(1) == "iso" )
            m_type = TypeISO;
        else
            m_type = TypeMPEG;

        m_name = exp.cap(2);
    }
    else
    {
        QFileInfo fileinfo( name );
    }

    std::cout << "match: " << exp.captureCount() << std::endl;
    std::cout << "exp.cap(0): " << exp.cap(0).toStdString() << std::endl;
    std::cout << "exp.cap(1): " << exp.cap(1).toStdString() << std::endl;
    std::cout << "exp.cap(2): " << exp.cap(2).toStdString() << std::endl;
}

QString dvd::MediaInput::name() const
{
    return m_name;
}

QString dvd::MediaInput::filename() const
{
    return m_name;
}

