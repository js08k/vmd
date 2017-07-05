#pragma once

#include "dvd/dvd.h"
#include "dvd/mediareceiver.h"

#include "message.h"
#include "datapackage.h"

class DVDSHARED_EXPORT dvd::NetworkContext
        : public dvd::MediaReceiver
{
    Q_OBJECT

public:
    NetworkContext(QObject* parent=0);
    void open( QString const& device );

public slots:
    void receive( gtqt::DataPackage<gtqt::MediaInfo> const& msg );
    void receive( gtqt::DataPackage<gtqt::MediaFrame> const& msg );

private:

};
