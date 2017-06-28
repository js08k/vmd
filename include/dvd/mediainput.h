#ifndef MEDIAINPUT_H
#define MEDIAINPUT_H

#include <QString>

namespace dvd { class MediaInput; }

class dvd::MediaInput
{
public:
    enum Type
    {
        TypeDVD,
        TypeMPEG
    };

    MediaInput( QString const& name );
    QString name() const;
    QString filename() const;

private:
    Type m_type;
    QString const m_name;
};

#endif // MEDIAINPUT_H
