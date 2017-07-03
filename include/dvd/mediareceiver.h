#pragma once

#include "dvd/dvd.h"
#include "dvd/mediacontext.h"

class DVDSHARED_EXPORT dvd::MediaReceiver
        : public dvd::MediaContext
{
    Q_OBJECT
public:
    MediaReceiver( QObject* parent=0 );

signals:
    void send( dvd::Control ) const;

public slots:
    void receive( dvd::MediaFrame const& );
    void pauseStream();
    void resumeStream();
    void menu();
    void highlight(dvd::MenuButton const&);
    void activate(dvd::MenuButton const&);

private:
};
