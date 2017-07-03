#pragma once

#include "dvd/dvd.h"
#include "dvd/mediareceiver.h"

class DVDSHARED_EXPORT dvd::NetworkContext
        : public dvd::MediaReceiver
{
    Q_OBJECT

public:
    NetworkContext(QObject* parent=0);
    void open( QString const& device );

private:

};
