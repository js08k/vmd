#include "connectionmanager.h"
#include "peerlink.h"

ConnectionManager::ConnectionManager(QObject* parent)
    : QObject(parent)
    , m_link( new gtqt::PeerLink(this) )
    , m_connected(false)
{

}

void ConnectionManager::listen(QHostAddress const&, quint16 )
{

}

void ConnectionManager::connectToHost(QHostAddress const&, quint16 )
{

}

void ConnectionManager::receive( gtqt::DataPackage<gtqt::NetPing> )
{

}
