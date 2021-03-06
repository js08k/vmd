#include "vmd.h"
#include "ui_vmd.h"
#include "linkmanager.h"

#include "peerlink.h"
#include "tcpsocket.h"
#include "dvd/streamplayer.h"
#include "dvd/mediainput.h"
#include "guiUtil/VideoWidget.h"

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
    , m_link(new LinkManager(this))
    , m_addrregexp("^(\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}):(\\d+)$")
    , m_mediaThread( new QThread )
    , m_player(0)
    , m_context(0)
{
    // Default the MediaInfo validity
    m_mediainfo.isNull = true;

    // Setup the Ui
    ui->setupUi(this);

    // Start the media thread
    m_mediaThread->start();

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

    connect( m_link, SIGNAL(receive(gtqt::DataPackage<gtqt::MediaInfo>)),
             this, SLOT(receive(gtqt::DataPackage<gtqt::MediaInfo>)) );

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
    if ( m_context )
    {
        m_context->deleteLater();
    }

    if ( m_mediaThread )
    {
        m_mediaThread->quit();
        m_mediaThread->wait();
        delete m_mediaThread;
    }

    delete ui;
    m_settings.sync();
}

void VMD::listen(QHostAddress const& address, quint16 port)
{
    static QString const msg("Listening on %1:%2");
    std::cout << msg.arg(address.toString(),QString::number(port)).toStdString() << std::endl;
    m_link->listen(address,port);
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
        m_link->connectToHost(host,port);
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

    for ( QListWidgetItem* item : items )
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

void VMD::clickPushButtonLoad()
{
    QList<QListWidgetItem*> items(ui->listWidgetLibrary->selectedItems());
    if ( items.empty() ) { return; }

    dvd::MediaInput input;
    if ( input.initialize( items.first()->text() ) )
    {
        // Set the MediaInfo
        m_mediainfo.isNull = false;
        m_mediainfo.msg.set_key( "mykey" );
        m_mediainfo.msg.set_title( input.name().toStdString() );

        std::cout << "New style media type deyphering" << std::endl;

        // Delete the previous context
        if ( m_context )
        { delete m_context; }

        // Create a new context according to the decyphered type
        m_context = input.create(this);
        if ( m_context )
        {
            m_context->moveToThread( m_mediaThread );

            m_link->transmit(gtqt::DataPackage<gtqt::MediaInfo>(m_mediainfo.msg));

            if ( m_player )
            { delete m_player; }

            m_player = new dvd::StreamPlayer(this);
            m_player->setVideoOutput(ui->widgetVideo);

            connect( m_context, SIGNAL(resolution(QSizeF)),
                     this, SLOT(resolution(QSizeF)) );

            connect( m_player, SIGNAL(pauseReadStream()),
                     m_context, SLOT(pauseStream()) );

            connect( m_player, SIGNAL(resumeReadStream()),
                     m_context, SLOT(resumeStream()) );

            connect( m_context, SIGNAL(stream(dvd::MediaFrame)),
                     m_player, SLOT(stream(dvd::MediaFrame)) );

            connect( m_context, SIGNAL(stream(dvd::MediaFrame)),
                     m_link, SLOT(transmit(dvd::MediaFrame)) );

            connect( ui->widgetVideo, SIGNAL(highlight(dvd::MenuButton)),
                     m_context, SLOT(highlight(dvd::MenuButton)) );

            connect( ui->widgetVideo, SIGNAL(activate(dvd::MenuButton)),
                     m_context, SLOT(activate(dvd::MenuButton)) );

            connect( m_context, SIGNAL(buttons(QList<dvd::MenuButton>)),
                     ui->widgetVideo, SLOT(buttons(QList<dvd::MenuButton>)) );

            connect( m_context, SIGNAL(title(QString)),
                     this, SLOT(setTitle(QString)) );

            m_context->open( input.path() );
        }
    }

    ui->pushButtonPausePlay->setText("Pause");
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

void VMD::resolution( QSizeF const& size )
{
    m_mediainfo.msg.set_width(size.width());
    m_mediainfo.msg.set_height(size.height());

    if ( !m_mediainfo.isNull && m_link->connected() )
    {
        m_link->transmit(gtqt::DataPackage<gtqt::MediaInfo>(m_mediainfo.msg));
    }

    ui->widgetVideo->resolution(size);
}
#include "dvd/networkcontext.h"
void VMD::receive( gtqt::DataPackage<gtqt::MediaInfo> const& msg )
{
    // Do not accept the message without a title or key
    if ( !msg->has_title() ) { return; }
    if ( !msg->has_key() ) { return; }

    // Verify the mediainfo has not changed title
    if ( !m_mediainfo.isNull &&
         msg->title() != m_mediainfo.msg.title() )
    {
        m_mediainfo.isNull = true;
    }

    // Copy the details of the message
    m_mediainfo.msg.set_title( msg->title() );
    m_mediainfo.msg.set_key( msg->key() );
    if ( msg->has_width() ) { m_mediainfo.msg.set_width( msg->width() ); }
    if ( msg->has_height() ) { m_mediainfo.msg.set_height( msg->height() ); }

    // This media is new to us, setup the output
    if ( m_mediainfo.isNull )
    {
        // Re-request the MediaInfo if we do not have heigh/width. This should
        // be a one-time request
        if ( !msg->has_height() || !msg->has_width() )
        {
            gtqt::DataPackage<gtqt::StreamRequest> request;
            request.data()->set_action( gtqt::StreamRequest::SendInfo );
            m_link->transmit( request );
        }

        // Set the Windowtitle to the peer stream title
        setTitle( msg->title().c_str() );

        // Delete the previous context
        if ( m_context )
        { delete m_context; }

        // Create a new context according to the decyphered type
        m_context = new dvd::NetworkContext(m_link);
        if ( m_context )
        {
            // Get rid of the old player
            if ( m_player )
            { delete m_player; }

            // Create a new player
            m_player = new dvd::StreamPlayer(this);
            m_player->setVideoOutput(ui->widgetVideo);
            connect( m_context, SIGNAL(resolution(QSizeF)),
                     this, SLOT(resolution(QSizeF)) );

            connect( m_player, SIGNAL(pauseReadStream()),
                     m_context, SLOT(pauseStream()) );

            connect( m_player, SIGNAL(resumeReadStream()),
                     m_context, SLOT(resumeStream()) );

            connect( m_context, SIGNAL(stream(dvd::MediaFrame)),
                     m_player, SLOT(stream(dvd::MediaFrame)) );

            connect( ui->widgetVideo, SIGNAL(highlight(dvd::MenuButton)),
                     m_context, SLOT(highlight(dvd::MenuButton)) );

            connect( ui->widgetVideo, SIGNAL(activate(dvd::MenuButton)),
                     m_context, SLOT(activate(dvd::MenuButton)) );

            static_cast<dvd::NetworkContext*>(m_context)->receive(msg);

            ui->pushButtonPausePlay->setText("Pause");
            ui->tabWidgetMain->setCurrentIndex(0);
        }
    }
}

void VMD::receive( gtqt::DataPackage<gtqt::StreamRequest> const& msg )
{
    switch ( msg->action() )
    {
    case gtqt::StreamRequest::SendInfo:
        if ( !m_mediainfo.isNull )
            m_link->transmit(gtqt::DataPackage<gtqt::MediaInfo>(m_mediainfo.msg));
        break;
    default:
        break;
    }
}

