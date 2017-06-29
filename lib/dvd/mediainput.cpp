#include "dvd/mediainput.h"
#include <iostream>

// libDvDRead
#include "dvdread/dvd_reader.h"

// Qt Includes
#include <QRegExp>
#include <QFileInfo>

dvd::MediaInput::MediaInput( QString const& name )
    : m_type(TypeDVD)
    , m_name(name)
{
    QRegExp exp( "^(dvd|file)://(\\S+)$" );
    exp.indexIn( name );

    if ( exp.captureCount() == 2 )
    {
        if ( exp.cap(1) == "dvd" )
            m_type = TypeDVD;
        else
            m_type = TypeMPEG;
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

