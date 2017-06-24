#include "vmd.h"
#include "ui_vmd.h"

#include "peerlink.h"
#include "tcpsocket.h"

#include <QRegExp>
#include <QHostAddress>
#include <QPalette>
#include <QFileDialog>
#include <QListWidgetItem>

#include <iostream>
#include <message.h>

VMD::VMD(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VMD)
    , m_addrregexp("^(\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}):(\\d+)$")
    , m_link( new gtqt::PeerLink(this) )
{
    m_settings.beginGroup("gui");
    ui->setupUi(this);

    // Restore Username
    ui->lineEditUserName->
            setText(m_settings.value("lineEditUserName").toString());

    // Restore the Host Address
    ui->lineEditHostAddress->
            setText(m_settings.value("lineEditHostAddress").toString());

    // Restore the Simulated Delay
    ui->lineEditSimulatedDelay->
            setText(m_settings.value("lineEditSimulatedDelay").toString());

    // Restore the Peer Address
    ui->lineEditPeerAddress->
            setText(m_settings.value("lineEditPeerAddress").toString());

    // Restore the items in the library
    ui->listWidgetLibrary->
            addItems(m_settings.value("listWidgetLibrary").toStringList());

    // Connect the Gui slots
    connect( ui->lineEditUserName, SIGNAL(editingFinished()),
             this, SLOT(lineEditUserNameFinished()) );
    connect( ui->lineEditHostAddress, SIGNAL(editingFinished()),
             this, SLOT(lineEditHostAddressFinished()) );
    connect( ui->lineEditSimulatedDelay, SIGNAL(editingFinished()),
             this, SLOT(lineEditSimulatedDelayFinished()) );
    connect( ui->lineEditPeerAddress, SIGNAL(editingFinished()),
             this, SLOT(lineEditPeerAddressFinished()) );
    connect( ui->pushButtonConnect, SIGNAL(clicked()),
             this, SLOT(clickPushButtonConnect()) );
    connect( ui->pushButtonAdd, SIGNAL(clicked()),
             this, SLOT(clickPushButtonAdd()) );
    connect( ui->pushButtonRemove, SIGNAL(clicked()),
             this, SLOT(clickPushButtonRemove()) );
    connect( ui->pushButtonLoad, SIGNAL(clicked()),
             this, SLOT(clickPushButtonLoad()) );

    connect( m_link, SIGNAL(receive(gtqt::DataPackage<gtqt::ClientType1>)),
           this, SLOT(receive(gtqt::DataPackage<gtqt::ClientType1>)) );

    QRegExp regexp(m_addrregexp);
    if ( regexp.indexIn(ui->lineEditHostAddress->text()) >= 0 )
    {
        std::cout << "cap(1): " << regexp.cap(1).toStdString() << std::endl;
        std::cout << "cap(2): " << regexp.cap(2).toStdString() << std::endl;

        QHostAddress const host( regexp.cap(1) );
        quint16 const port( regexp.cap(2).toInt() );
        listen( host, port );
    }
}

VMD::~VMD()
{
    delete ui;
    m_settings.sync();
}

void VMD::listen(QHostAddress const& address, quint16 port)
{
    m_link->close();
    m_link->listen( address, port );

    static QString const msg("Listening on %1:%2");
    std::cout << msg.arg(address.toString(),QString::number(port)).toStdString() << std::endl;
}

void VMD::lineEditUserNameFinished()
{
    static QString const key("lineEditUserName");
    QString const orig( m_settings.value(key).toString() );
    QString const value( ui->lineEditUserName->text() );

    if ( orig != value )
    {
        // TODO Enforce a validation of the username before accepting

        m_settings.setValue( key, QVariant::fromValue(value) );
        static QString const msg( "Saved %1: \'%2\'" );
        std::cout << msg.arg( key, value ).toStdString() << std::endl;
    }
}

void VMD::lineEditHostAddressFinished()
{
    // Clear out any error color
    ui->lineEditHostAddress->setPalette(palette());

    static QString const key("lineEditHostAddress");
    QString const orig( m_settings.value(key).toString() );
    QString const value( ui->lineEditHostAddress->text() );

    if ( orig == value ) { return; }

    QRegExp regexp(m_addrregexp);
    if ( regexp.indexIn(value) >= 0 )
    {
        std::cout << "cap(1): " << regexp.cap(1).toStdString() << std::endl;
        std::cout << "cap(2): " << regexp.cap(2).toStdString() << std::endl;

        QHostAddress const host( regexp.cap(1) );
        quint16 const port( regexp.cap(2).toInt() );
        listen( host, port );

        m_settings.setValue( key, QVariant::fromValue(value) );
        static QString const msg( "Saved %1: \'%2\'" );
        std::cout << msg.arg( key, value ).toStdString() << std::endl;
    }
    else
    {
        // Set the background of the lineEditHostAddress to an error color
        QPalette errpalette( palette() );
        errpalette.setBrush( QPalette::Base, QBrush(Qt::red) );
        ui->lineEditHostAddress->setPalette(errpalette);
    }
}

void VMD::lineEditSimulatedDelayFinished()
{
    static QString const key("lineEditSimulatedDelay");
    QString const orig( m_settings.value(key).toString() );
    QString const value( ui->lineEditSimulatedDelay->text() );

    if ( orig == value ) { return; }

    // TODO Enforce a validation of the username before accepting

    m_settings.setValue( key, QVariant::fromValue(value) );
    static QString const msg( "Saved %1: \'%2\'" );
    std::cout << msg.arg( key, value ).toStdString() << std::endl;
}

void VMD::lineEditPeerAddressFinished()
{
    static QString const key("lineEditPeerAddress");
    QString const orig( m_settings.value(key).toString() );
    QString const value( ui->lineEditPeerAddress->text() );

    if ( orig == value ) { return; }

    // TODO Enforce a validation of the username before accepting

    m_settings.setValue( key, QVariant::fromValue(value) );
    static QString const msg( "Saved %1: \'%2\'" );
    std::cout << msg.arg( key, value ).toStdString() << std::endl;
}
#include <QTimer>
void VMD::clickPushButtonConnect()
{
    QString const value( ui->lineEditPeerAddress->text() );

    QRegExp regexp(m_addrregexp);
    if ( regexp.indexIn(value) >= 0 )
    {
        std::cout << "cap(1): " << regexp.cap(1).toStdString() << std::endl;
        std::cout << "cap(2): " << regexp.cap(2).toStdString() << std::endl;

        QHostAddress const host( regexp.cap(1) );
        quint16 const port( regexp.cap(2).toInt() );
        m_link->connectToHost( host, port );

        QTimer::singleShot( 1000, this, SLOT(timeout()) );
    }
}

void VMD::clickPushButtonAdd()
{
    QString file( QFileDialog::getOpenFileName(this,
                                               "OpenFile",
                                               QDir::homePath(),
                                               "Multimedia files(*)") );

    if ( file.isEmpty() )
    { return; }

    QStringList list;
    for ( int i(0); i < ui->listWidgetLibrary->count(); ++i )
    {
        QListWidgetItem* item( ui->listWidgetLibrary->item(i) );
        if ( item )
        { list << item->text(); }
    }

    if ( !list.contains(file) )
    {
        ui->listWidgetLibrary->addItem(file);
        list << file;
        m_settings.setValue("listWidgetLibrary", list);
    }
}

void VMD::clickPushButtonRemove()
{
    QList<QListWidgetItem*> items(ui->listWidgetLibrary->selectedItems());
    if ( items.empty() ) { return; }

    foreach ( QListWidgetItem* item, items )
    {
        delete item;
    }

    QStringList list;
    for ( int i(0); i < ui->listWidgetLibrary->count(); ++i )
    {
        QListWidgetItem* item( ui->listWidgetLibrary->item(i) );
        if ( item )
        { list << item->text(); }
    }

    m_settings.setValue("listWidgetLibrary", list);
}

void VMD::clickPushButtonLoad()
{
    ui->tabWidgetMain->setCurrentIndex(0);
}

void VMD::receive( gtqt::DataPackage<gtqt::ClientType1> const& msg )
{
    Q_UNUSED(msg)
    std::cout << "Received gtqt::ClientType1" << std::endl;
}

void VMD::timeout()
{
    std::cout << "TIMEOUT! Sending gtqt::PeerInformation" << std::endl;
    gtqt::DataPackage<gtqt::ClientType1> msg;
    msg.data()->set_data( qPrintable("Test Message") );
    m_link->transmit(msg);
}

