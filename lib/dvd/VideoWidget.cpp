#include "dvd/VideoWidget.h"

#include <QMouseEvent>
#include <QBitmap>
#include <QPixmap>
#include <QTimer>
#include <QCoreApplication>
#include <QPainter>
#include "dvd/menubutton.h"
#include <iostream>
#include <QPalette>

namespace dvd { class UiOverlay; }

void dvd::UiOverlay::paint(QPainter* p, QStyleOptionGraphicsItem const*, QWidget* w)
{
    p->setPen( Qt::yellow );

    for (dvd::MenuButton button : m_buttons)
    {
        p->setPen(Qt::red);
        p->drawRect( button.mapToScreen(m_rect.size()));
    }
}

QRectF dvd::UiOverlay::boundingRect() const
{
    return m_rect;
}

void dvd::UiOverlay::setRect( QRect r )
{
    m_rect = r;
}

void dvd::UiOverlay::buttons( QList<dvd::MenuButton> const& buttons )
{
    m_buttons = buttons;
}


QWidget* m_uioverlay;

VideoWidget::VideoWidget(QWidget* parent)
    : QGraphicsView(parent)
    , m_vOutput( new QGraphicsVideoItem )
    , m_uioverlay( new dvd::UiOverlay )
{
    // Create and set the scene
    setScene( new QGraphicsScene(this) );
    scene()->setSceneRect( rect() );

    // Add the video item to the scene
    connect( m_vOutput, SIGNAL(nativeSizeChanged(QSizeF)), SLOT(videoNativeSizeChanged(QSizeF)) );
    scene()->addItem(m_vOutput);

    // Add the ui overlay to the scene
    scene()->addItem(m_uioverlay);
    m_uioverlay->setRect( rect() );
}

void VideoWidget::buttons( QList<dvd::MenuButton> const& buttons )
{
    m_uioverlay->buttons(buttons);
    m_buttons = buttons;
    update();
}

void VideoWidget::resizeEvent(QResizeEvent* e)
{
    QGraphicsView::resizeEvent(e);
}

void VideoWidget::mouseMoveEvent( QMouseEvent* event )
{
    std::cout << "MouseTracking: " << testAttribute(Qt::WA_MouseTracking) << std::endl;
    QWidget::mouseMoveEvent(event);
}

void VideoWidget::mousePressEvent( QMouseEvent* e )
{
    QWidget::mousePressEvent(e);
    std::cout << "VideoWidget::mousePressEvent()" << std::endl;

    // Cycle through our buttons
    QCursor c( cursor() );
    for ( dvd::MenuButton btn : m_buttons )
    {
        if ( btn.mapToScreen(size()).contains(QWidget::mapFromGlobal(c.pos())) )
        {
            std::cout << "emit activate()" << std::endl;
            emit activate(btn);
            break;
        }
    }
}

#include <QRectF>
void VideoWidget::cursorSpy()
{

    // Default check the cursor in 10 mS
    int interval(10);

    QCursor c( cursor() );
    if ( c.pos() != m_lastCursorPosition )
    {
        // The cursor just moved!
        m_lastCursorMovement.start();
        m_lastCursorPosition = c.pos();

        dvd::MenuButton selected;

        // Cycle through our buttons
        for ( dvd::MenuButton btn : m_buttons )
        {
            if ( btn.mapToScreen(size()).contains( QWidget::mapFromGlobal(c.pos()) ) )
            {
                selected = btn;
                break;
            }
        }

        // Set the cursor to default if it was non default
        if ( selected.isNull() )
        {
            setCursor( QCursor() );
        }
        else
        {
            setCursor( QCursor(Qt::PointingHandCursor) );
            emit highlight(selected);
        }
    }
    else if ( m_lastCursorMovement.elapsed() > 5000 )
    {
        // The cursor has not moved in a while, set it to invisible
        QPixmap cursor(1,1);
        cursor.fill( Qt::transparent );
        setCursor( QCursor( cursor ) );

        // Relax the interval since the cursor has not moved lately
        interval = 100;
    }

    // Check the cursor again in 'interval' mS
    QTimer::singleShot( interval, this, SLOT(cursorSpy()) );
}

void VideoWidget::videoNativeSizeChanged( QSizeF size )
{
    setMinimumSize( size.toSize() );
    scene()->setSceneRect( QRect( QPoint(0,0), size.toSize() ) );
    m_vOutput->setSize( size );
    m_uioverlay->setRect( QRect( QPoint(0,0), size.toSize() ) );
}
