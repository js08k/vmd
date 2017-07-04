#pragma once

#include <QObject>
#include <QVector>
#include <QHostAddress>

#include "message.h"
#include "datapackage.h"
#include "peerlink.h"

class LinkManager
        : public QObject
{
    Q_OBJECT
public:
    LinkManager(QObject* parent = 0);
    inline bool connected() const;
    void listen(QHostAddress const&, quint16 port);
    void connectToHost(QHostAddress const&, quint16 port);

    template <typename T>
    inline void transmit( T const& ) const;

signals:
    void isConnected() const;
    void isDisconnected() const;
    void receive( gtqt::DataPackage<gtqt::MediaInfo> const& ) const;
    void receive( gtqt::DataPackage<gtqt::MediaFrame> const& ) const;
    void receive( gtqt::DataPackage<gtqt::StreamRequest> const& ) const;

private slots:
    void connected(QHostAddress const&, quint16);
    void disconnected(QHostAddress const&, quint16);
    void error(QAbstractSocket::SocketError e);

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

bool LinkManager::connected() const
{ return m_peer.connected; }

template <typename T>
void LinkManager::transmit( T const& data ) const
{
    if ( m_peer.connected )
    { m_link->transmit(data); }
}
