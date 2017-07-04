#include "linkmanager.h"
#include "peerlink.h"
#include <iostream>

LinkManager::LinkManager(QObject* parent)
    : QObject(parent)
    , m_link( new gtqt::PeerLink(this) )
{
    m_peer.connected = false;

    connect( m_link, SIGNAL(connected(QHostAddress,quint16)),
             this, SLOT(connected(QHostAddress,quint16)) );
    connect( m_link, SIGNAL(disconnected(QHostAddress,quint16)),
             this, SLOT(disconnected(QHostAddress,quint16)) );
    connect( m_link, SIGNAL(error(QAbstractSocket::SocketError)),
             this, SLOT(error(QAbstractSocket::SocketError)) );

    connect( m_link, SIGNAL(receive(gtqt::DataPackage<gtqt::MediaInfo>)),
             this, SIGNAL(receive(gtqt::DataPackage<gtqt::MediaInfo>)) );
}

void LinkManager::listen(QHostAddress const& addr, quint16 port )
{
    std::cout << "LinkManager::listen" << std::endl;
    m_link->listen(addr,port);
}

void LinkManager::connectToHost(QHostAddress const& addr, quint16 port)
{
    m_link->connectToHost(addr,port);
}

void LinkManager::connected(QHostAddress const& addr, quint16 port)
{
    std::cout << "Connected on " << addr.toString().toStdString()
              << ":" << port << std::endl;

    if ( m_peer.connected )
        std::cerr << "ERROR: Already connected!" << std::endl;

    // Set the connected flag, and the cooresponding data
    m_peer.connected = true;
    m_peer.address = addr;
    m_peer.port = port;
}

void LinkManager::disconnected(QHostAddress const& addr, quint16 port)
{
    std::cout << "Disconnected on " << addr.toString().toStdString()
              << ":" << port << std::endl;

    if ( !m_peer.connected )
        std::cerr << "ERROR: Was not connected!" << std::endl;

    // Clear the connected flag
    m_peer.connected = false;
}

void LinkManager::error(QAbstractSocket::SocketError e)
{
    std::cout << "Socket Error: " << e << std::endl;
}

void LinkManager::receive( gtqt::DataPackage<gtqt::NetPing> )
{

}
