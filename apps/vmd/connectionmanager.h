#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QObject>
#include <QVector>
#include <QHostAddress>

#include "message.h"
#include "datapackage.h"
#include "peerlink.h"

class ConnectionManager
        : public QObject
{
    Q_OBJECT
public:
    ConnectionManager(QObject* parent = 0);
    inline bool connected() const;
    void listen(QHostAddress const&, quint16 port);
    void connectToHost(QHostAddress const&, quint16 port);

    template <typename T>
    inline void transmit( T const& ) const;

signals:
    void receive( gtqt::DataPackage<gtqt::MediaInfo> const& ) const;

private slots:
    void connected(QHostAddress const&, quint16);
    void disconnected(QHostAddress const&, quint16);

    void receive( gtqt::DataPackage<gtqt::NetPing> );

private:
    gtqt::PeerLink* m_link;

    struct Peer
    {
        bool connected;
        QHostAddress address;
        quint16 port;
    } m_peer;
};

bool ConnectionManager::connected() const
{ return m_peer.connected; }

template <typename T>
void ConnectionManager::transmit( T const& data ) const
{
    if ( m_peer.connected )
    { m_link->transmit(data); }
}

#endif // CONNECTIONMANAGER_H
