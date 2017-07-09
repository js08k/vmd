#include "guiUtil/UiOverlay.h"
#include "ui_uioverlay.h"

#include "dvd/menubutton.h"
#include "dvd/mediainput.h"

#include <QPainter>
#include <QFileDialog>
#include <QMouseEvent>
#include <QPushButton>

gui::UiOverlay::UiOverlay(QWidget *parent)
    : QWidget(parent)
    , activate( [](dvd::MenuButton){} )
    , highlight( [](dvd::MenuButton){} )
    , cursorIsActive( [](){} )
    , ui( new Ui::UiOverlay )
    , m_ldvd( new QPushButton(this) )
    , m_load( new QPushButton(this) )
    , m_menu( new QPushButton(this) )
{
    ui->setupUi(this);

    // Set the default Ui state
    ui->labelStatus->setVisible(false);
    ui->lineEditPeerAddr->setVisible(false);
    ui->pushButtonConnect->setVisible(false);
    ui->lineEditHostAddr->setVisible(false);

    setWindowFlags( Qt::X11BypassWindowManagerHint );
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_MouseTracking, true);

    // Initialize the ldvd button
    m_ldvd->resize( QSize(23,23) );
    m_ldvd->setIcon( QIcon::fromTheme("media-optical-dvd") );
    connect( m_ldvd, SIGNAL(clicked()), SLOT(ldvdClicked()) );

    // Initialize the load button
    m_load->resize( QSize(23,23) );
    m_load->setIcon( QIcon::fromTheme("folder-open") );
    connect( m_load, SIGNAL(clicked()), SLOT(loadClicked()) );

    // Initialize the menu button
    m_menu->resize( QSize(23,23) );
    m_menu->setCheckable( true );
    m_menu->setIcon( QIcon::fromTheme("preferences-system") );
    connect( m_menu, SIGNAL(toggled(bool)), SLOT(menuToggled(bool)) );

    connect( ui->pushButtonConnect, SIGNAL(clicked()), SIGNAL(clickedConnect()) );
    connect( ui->lineEditPeerAddr, SIGNAL(editingFinished()), SLOT(peerAddrEditingFinished()) );
    connect( ui->lineEditHostAddr, SIGNAL(editingFinished()), SLOT(hostAddrEditingFinished()) );
}

gui::UiOverlay::~UiOverlay()
{
}

void gui::UiOverlay::menuToggled(bool checked)
{
    ui->labelStatus->setVisible(checked);
    ui->lineEditPeerAddr->setVisible(checked);
    ui->pushButtonConnect->setVisible(checked);
    ui->lineEditHostAddr->setVisible(checked);
}
#include <iostream>
void gui::UiOverlay::ldvdClicked()
{
#ifdef Q_OS_UNIX
    QString const dev( "dvd:///dev/dvd" );
#else
    QString const dev( "dvd://E:/" );
#endif
    std::cout << "Load dev: " << dev.toStdString() << std::endl;
    emit loadMedia( "dvd:///dev/dvd" );
}

void gui::UiOverlay::loadClicked()
{
    QString file( QFileDialog::getOpenFileName(this,
                                               "OpenFile",
                                               QDir::homePath(),
                                               "Multimedia files(*)") );
    setVisible(true);

    if ( file.isEmpty() ) { return; }
    dvd::MediaInput input;
    if ( input.initialize(file) )
    {
        std::cout << "Successful load " << input.path().toStdString() << std::endl;
        emit loadMedia( input.path() );
    }
    else
    {
        std::cout << "Failed load " << file.toStdString() << std::endl;
    }
}

void gui::UiOverlay::peerAddrEditingFinished()
{
    emit peerAddressChanged( ui->lineEditPeerAddr->text() );
}

void gui::UiOverlay::hostAddrEditingFinished()
{
    emit peerAddressChanged( ui->lineEditHostAddr->text() );
}

void gui::UiOverlay::resize( QSize size )
{
    qreal const buffer(10);

    // Move the menu button
    QRect menu( m_menu->rect() );
    menu.moveTopRight( QPoint(size.width()-buffer, buffer) );
    m_menu->move( menu.topLeft() );

    // Move the load button
    QRect load( m_load->rect() );
    load.moveTopRight( QPoint(menu.left() - 2, buffer) );
    m_load->move( load.topLeft() );

    // Move the ldvd button
    QRect ldvd( m_ldvd->rect() );
    ldvd.moveTopRight( QPoint(load.left() - 2, buffer) );
    m_ldvd->move( ldvd.topLeft() );

    QWidget::resize(size);
}

void gui::UiOverlay::paintEvent( QPaintEvent* e )
{
    QRectF const canvas( rect().adjusted(0,0,-1,-1) );
    QWidget::paintEvent(e);

    QPainter painter(this);

    painter.setBrush( Qt::transparent );
    painter.setPen( Qt::red );
    painter.drawRect( canvas );

    painter.setPen( Qt::blue );
    for ( dvd::MenuButton button : m_buttons )
    {
        painter.drawRect( button.mapToScreen(rect().size()) );
    }
}

void gui::UiOverlay::setButtons( QList<dvd::MenuButton> const& buttons)
{
    m_buttons = buttons;
    update();
}

void gui::UiOverlay::cursorIsInactive()
{
    if ( !m_menu->isChecked() )
    {
        // Hide the cursor
        QPixmap pm(1,1);
        pm.fill( Qt::transparent );
        setCursor( QCursor(pm) );

        m_ldvd->setVisible(false);
        m_load->setVisible(false);
        m_menu->setVisible(false);
    }
}

void gui::UiOverlay::setConnected(bool isConnected)
{
    if ( isConnected )
    {
        ui->labelStatus->setText( "Connected" );
        ui->pushButtonConnect->setText("Disconnect");
    }
    else
    {
        ui->labelStatus->setText( "Disconnected" );
        ui->pushButtonConnect->setText("Connect");
    }
}

void gui::UiOverlay::setPeerAddress( QString const& text )
{
    ui->lineEditPeerAddr->setText(text);
}

void gui::UiOverlay::setHostAddress( QString const& text )
{
    ui->lineEditHostAddr->setText(text);
}

void gui::UiOverlay::mouseMoveEvent( QMouseEvent* e )
{
    cursorIsActive();

    // Menu is checked, skip extra behaviours
    if ( m_menu->isChecked() ) { return; }

    m_ldvd->setVisible(true);
    m_load->setVisible(true);
    m_menu->setVisible(true);

    bool canClick(false);

    for ( dvd::MenuButton const& btn : m_buttons )
    {
        if ( btn.mapToScreen( size() ).contains(e->pos()) )
        {
            canClick = true;
            highlight(btn);
            break;
        }
    }

    if ( canClick )
        setCursor( Qt::PointingHandCursor );
    else
        setCursor( Qt::ArrowCursor );
}

void gui::UiOverlay::mousePressEvent( QMouseEvent* e )
{
    for ( dvd::MenuButton const& btn : m_buttons )
    {
        if ( btn.mapToScreen( size() ).contains(e->pos()) )
        {
            activate(btn);
            break;
        }
    }
}
