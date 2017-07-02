#pragma once

// Project Includes
#include "dvd/dvd.h"

// Qt Includes
#include <QObject>

class DVDSHARED_EXPORT dvd::MediaContext
        : public QObject
{
    Q_OBJECT
public:
    explicit MediaContext(QObject* parent = 0);
    virtual ~MediaContext();
    virtual void open( QString const& ) = 0;
    inline dvd::MediaState state() const;

signals:
    void title(QString const& ) const;
    void stream(dvd::MediaFrame const& ) const;
    void mediaStateChange(dvd::MediaState const&) const;

public slots:
    virtual void pauseStream() = 0;
    virtual void resumeStream() = 0;
    virtual void menu() = 0;
    virtual void highlight(dvd::MenuButton const&) = 0;
    virtual void activate(dvd::MenuButton const&) = 0;

protected:
    void setMediaState( dvd::MediaState const& );

private:
    dvd::MediaState m_state;
};

dvd::MediaState dvd::MediaContext::state() const
{
    return m_state;
}
