#include "dvd/VideoWidget.h"

#include <QMouseEvent>
#include <QBitmap>
#include <QPixmap>
#include <QTimer>
#include <QCoreApplication>
#include <QPainter>
#include "dvd/menubutton.h"
#include <iostream>

VideoWidget::VideoWidget(QWidget* parent)
    : QVideoWidget(parent)
{
    cursorSpy();
}

VideoWidget::~VideoWidget()
{

}

void VideoWidget::buttons( QList<dvd::MenuButton> const& buttons )
{
    m_buttons = buttons;
    update();
}

void VideoWidget::mouseMoveEvent( QMouseEvent* event )
{
//    std::cout << "MouseTracking: " << testAttribute(Qt::WA_MouseTracking) << std::endl;
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
