#pragma once

#include "dvd/dvd.h"
#include <QObject>
#include "dvd/mediacontext.h"
#include "message.h"

class dvd::MediaSender
        : public dvd::MediaContext
{
    Q_OBJECT
public:
    MediaSender( QObject* parent = 0 );

signals:
    void send( dvd::MediaFrame const& ) const;

public slots:
    void receive( gtqt::StreamRequest const& );

private:
    void tryPauseStream();
    void tryResumeStream();

private:
    bool m_streamPaused;
};
