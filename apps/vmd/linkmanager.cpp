#include "linkmanager.h"
#include "peerlink.h"
#include <iostream>

LinkManager::LinkManager(QObject* parent)
    : QObject(parent)
    , m_link( new gtqt::PeerLink(this) )
{
    m_peer.connected = false;
}

void LinkManager::listen(QHostAddress const& addr, quint16 port )
{
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

void LinkManager::receive( gtqt::DataPackage<gtqt::NetPing> )
{

}
