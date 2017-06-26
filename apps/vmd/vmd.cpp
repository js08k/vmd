#include "vmd.h"
#include "ui_vmd.h"

#include "peerlink.h"
#include "tcpsocket.h"

#include <QRegExp>
#include <QHostAddress>
#include <QPalette>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QThread>
#include <QMediaPlayer>
#include <QTimer>

#include <iostream>
#include <message.h>

VMD::VMD(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VMD)
    , m_addrregexp("^(\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}):(\\d+)$")
    , m_link( new gtqt::PeerLink(this) )
    , m_mediaThread( new QThread )
    , m_mediaContext( new DvDContext() )
    , m_player(0)
{
    // Setup the Ui
    ui->setupUi(this);

    // Move the MediaContext to a thread, and start the thread
    m_mediaContext->moveToThread(m_mediaThread);
    m_mediaThread->start();

//    connect( &m_buffer, SIGNAL(pauseReadStream()), m_mediaContext, SLOT(pause()) );
//    connect( &m_buffer, SIGNAL(resumeReadStream()), m_mediaContext, SLOT(resume()) );
//    connect( m_mediaContext, SIGNAL(stream(QByteArray,dvd::StreamAction)),
//             &m_buffer, SLOT(stream(QByteArray,dvd::StreamAction)) );

    connect( ui->widgetVideo, SIGNAL(highlight(MenuButton)),
             m_mediaContext, SLOT(highlight(MenuButton)) );
    connect( ui->widgetVideo, SIGNAL(activate(MenuButton)),
             m_mediaContext, SLOT(activate(MenuButton)) );
    connect( m_mediaContext, SIGNAL(buttons(QList<MenuButton>)),
             ui->widgetVideo, SLOT(buttons(QList<MenuButton>)) );
    connect( m_mediaContext, SIGNAL(title(QString)),
             this, SLOT(setTitle(QString)) );

    m_settings.beginGroup("gui");

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

    // Restore the DVD Beginning Title
    ui->spinBoxStartTitle->
            setValue(m_settings.value("spinBoxStartTitle").toInt());

    // Restore the DVD Beginning Chapter
    ui->spinBoxStartChapter->
            setValue(m_settings.value("spinBoxStartChapter").toInt());

    // Connect the Gui slots
    connect( ui->lineEditUserName, SIGNAL(editingFinished()),
             this, SLOT(lineEditUserNameFinished()) );
    connect( ui->lineEditHostAddress, SIGNAL(editingFinished()),
             this, SLOT(lineEditHostAddressFinished()) );
    connect( ui->lineEditSimulatedDelay, SIGNAL(editingFinished()),
             this, SLOT(lineEditSimulatedDelayFinished()) );
    connect( ui->lineEditPeerAddress, SIGNAL(editingFinished()),
             this, SLOT(lineEditPeerAddressFinished()) );
    connect( ui->spinBoxStartTitle, SIGNAL(valueChanged(int)),
             this, SLOT(dvdStartTitleChanged(int)) );
    connect( ui->spinBoxStartChapter, SIGNAL(valueChanged(int)),
             this, SLOT(dvdStartChapterChanged(int)) );
    connect( ui->pushButtonConnect, SIGNAL(clicked()),
             this, SLOT(clickPushButtonConnect()) );
    connect( ui->pushButtonAdd, SIGNAL(clicked()),
             this, SLOT(clickPushButtonAdd()) );
    connect( ui->pushButtonRemove, SIGNAL(clicked()),
             this, SLOT(clickPushButtonRemove()) );
    connect( ui->pushButtonLoad, SIGNAL(clicked()),
             this, SLOT(clickPushButtonLoad()) );
    connect( ui->pushButtonPausePlay, SIGNAL(clicked()),
             this, SLOT(clickPushButtonPlayPause()) );

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

    // Stop the media thread and wait for the thread to join
    m_mediaContext->deleteLater();
    m_mediaThread->quit();
    m_mediaThread->wait();
    delete m_mediaThread;

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

void VMD::dvdStartTitleChanged(int value)
{
    m_settings.setValue("spinBoxStartTitle", value);
}

void VMD::dvdStartChapterChanged(int value)
{
    m_settings.setValue("spinBoxStartChapter", value);
}

void VMD::clickPushButtonConnect()
{
    QString const value( ui->lineEditPeerAddress->text() );

    QRegExp regexp(m_addrregexp);
    if ( regexp.indexIn(value) >= 0 )
    {
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
    {   delete item; }

    QStringList list;
    for ( int i(0); i < ui->listWidgetLibrary->count(); ++i )
    {
        QListWidgetItem* item( ui->listWidgetLibrary->item(i) );
        if ( item )
        { list << item->text(); }
    }

    m_settings.setValue("listWidgetLibrary", list);
}
#include "dvd/streamplayer.h"
void VMD::clickPushButtonLoad()
{
    QList<QListWidgetItem*> items(ui->listWidgetLibrary->selectedItems());
    if ( items.empty() ) { return; }

    m_mediaContext->open( items.first()->text() );
    ui->pushButtonPausePlay->setText("Pause");

    if ( m_player )
    { delete m_player; }

    m_player = new dvd::StreamPlayer(this);
    m_player->setVideoOutput(ui->widgetVideo);

    connect( m_player, SIGNAL(pauseReadStream()), m_mediaContext, SLOT(pause()) );
    connect( m_player, SIGNAL(resumeReadStream()), m_mediaContext, SLOT(resume()) );
    connect( m_mediaContext, SIGNAL(stream(dvd::MediaFrame)),
             m_player, SLOT(stream(dvd::MediaFrame)) );

//    m_player = new QMediaPlayer(this, QMediaPlayer::StreamPlayback );
//    m_player->setMedia( QMediaContent(), &m_buffer );
//    m_player->setVideoOutput(ui->widgetVideo);
//    connect( &m_buffer, SIGNAL(ready()), m_player, SLOT(play()) );

    ui->tabWidgetMain->setCurrentIndex(0);
}

void VMD::clickPushButtonPlayPause()
{
    QString const text( ui->pushButtonPausePlay->text() );

    if ( text == "Play" )
    {
        if ( m_player )
            m_player->play();

        ui->pushButtonPausePlay->setText("Pause");
    }
    else if ( text == "Pause" )
    {
        if ( m_player )
            m_player->pause();

        ui->pushButtonPausePlay->setText("Play");
    }
    else
    {
        ui->pushButtonPausePlay->setText("Play");
    }
}

void VMD::setTitle( QString const& title )
{
    setWindowTitle(title);
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

