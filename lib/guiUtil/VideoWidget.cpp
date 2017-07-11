#include "guiUtil/VideoWidget.h"
#include "guiUtil/UiOverlay.h"

#include <QTimer>
#include <QVBoxLayout>
#include <QMoveEvent>

gui::VideoWidget::VideoWidget(QWidget* parent)
    : QVideoWidget(parent)
    , m_overlay( new gui::UiOverlay )
    , m_cursorActiveTimer( new QTimer(this) )
{
    m_overlay->setVisible(true);
    setAspectRatioMode(Qt::IgnoreAspectRatio);

    connect( m_overlay, SIGNAL(peerAddressChanged(QString)), this, SIGNAL(peerAddressChanged(QString)) );
    connect( m_overlay, SIGNAL(hostAddressChanged(QString)), this, SIGNAL(hostAddressChanged(QString)) );
    connect( m_overlay, SIGNAL(loadMedia(QString)), this, SIGNAL(loadMedia(QString)) );
    m_overlay->setActivate([&](dvd::MenuButton btn){ emit activate(btn); });
    m_overlay->setHighlight([&](dvd::MenuButton btn){ emit highlight(btn); });
    m_overlay->setCursorIsActive([&](){ m_cursorActiveTimer->start(); });

    // Configure and connect the timer to control the visibility of the
    // cursor on the overlay
    m_cursorActiveTimer->setInterval( 5000 );
    m_cursorActiveTimer->setSingleShot(true);
    connect( m_cursorActiveTimer, SIGNAL(timeout()),
             this, SLOT(cursorIsInactive()) );

    setFocusPolicy( Qt::StrongFocus );
}

gui::VideoWidget::~VideoWidget()
{
    if ( m_overlay )
    {
        delete m_overlay;
        m_overlay = 0;
    }
}

void gui::VideoWidget::setPeerAddress( QString const& addr)
{
    m_overlay->setPeerAddress(addr);
}

void gui::VideoWidget::setHostAddress( QString const& addr)
{
    m_overlay->setHostAddress(addr);
}

void gui::VideoWidget::resolution(QSizeF const& size)
{
    // Calculate a rectagle to represent the resolution with a 1 pixel boundry
    QRectF const rect( QPointF(), size+QSizeF(2,2) );

    // Set the widget size
    setMinimumSize( rect.size().toSize() );

    // Set the overlay size
    m_overlay->setMinimumSize( rect.size().toSize() );
}

void gui::VideoWidget::buttons( QList<dvd::MenuButton> const& btns )
{
    m_overlay->setButtons(btns);
}
void gui::VideoWidget::closeEvent(QCloseEvent* e)
{
    if ( m_overlay )
    {
        delete m_overlay;
        m_overlay = 0;
    }

    QVideoWidget::closeEvent(e);
}

void gui::VideoWidget::moveEvent(QMoveEvent* e)
{
    QVideoWidget::moveEvent(e);
    m_overlay->move( geometry().topLeft() );
}

void gui::VideoWidget::hideEvent(QHideEvent* e)
{
    QVideoWidget::hideEvent(e);

    if ( m_overlay )
    { m_overlay->setVisible(false); }
}

void gui::VideoWidget::showEvent(QShowEvent* e)
{
    QVideoWidget::showEvent(e);

    if ( m_overlay )
    { m_overlay->setVisible(true); }
}

void gui::VideoWidget::resizeEvent(QResizeEvent* e)
{
    QVideoWidget::resizeEvent(e);

    if ( m_overlay )
    { m_overlay->resize(e->size()); }
}

void gui::VideoWidget::focusOutEvent(QFocusEvent* e)
{
    QVideoWidget::focusOutEvent(e);
//    if ( m_overlay )
//    { m_overlay->setVisible(false); }
}

void gui::VideoWidget::focusInEvent(QFocusEvent* e)
{
    QVideoWidget::focusInEvent(e);
//    if ( m_overlay )
//    {
//        m_overlay->move( geometry().topLeft() );
//        m_overlay->resize(size());
//        m_overlay->setVisible(true);
//    }
}

void gui::VideoWidget::cursorIsInactive()
{
    m_overlay->cursorIsInactive();
}
