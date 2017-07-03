#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QObject>
#include <QVector>
#include <QHostAddress>

#include "message.h"
#include "datapackage.h"

namespace gtqt { class PeerLink; }

class ConnectionManager
        : public QObject
{
    Q_OBJECT
public:
    ConnectionManager(QObject* parent = 0);
    inline bool connected() const;
    void listen(QHostAddress const&, quint16 port);
    void connectToHost(QHostAddress const&, quint16 port);

signals:

public slots:

private slots:
    void receive( gtqt::DataPackage<gtqt::NetPing> );

private:
    struct Peer
    {
        QHostAddress address;
        quint16 port;
    };

    gtqt::PeerLink* m_link;
    bool m_connected;
    QVector<Peer> m_peers;
};

bool ConnectionManager::connected() const
{ return bool(m_peers.length() > 0); }

#endif // CONNECTIONMANAGER_H
