#ifndef MEDIAINPUT_H
#define MEDIAINPUT_H

#include "dvd/dvd.h"

#include <QString>
#include <QObject>
#include <QSharedPointer>

#include <functional>

namespace dvd { class MediaInput; }

class dvd::MediaInput
{
public:
    MediaInput();
    bool initialize( QString const& );
    dvd::MediaContext* create( QObject* parent = 0 ) const;
    QString name() const;
    QString path() const;

private:
    bool tryDVD(QString const&);
    bool tryFile(QString const&);
    bool tryPeer(QString const&);

private:
    dvd::MediaType m_type;
    QString m_path;
    QString m_name;
};

#endif // MEDIAINPUT_H
