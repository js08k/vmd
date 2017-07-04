#include "guiUtil/uioverlay.h"

#include "dvd/menubutton.h"

#include <QCursor>
#include <QPainter>
#include <QGraphicsSceneHoverEvent>

#include <iostream>

dvd::UiOverlay::UiOverlay()
    : activate( [](dvd::MenuButton){} )
    , highlight( [](dvd::MenuButton){} )
    , cursorIsActive( [](){} )
{
    setAcceptedMouseButtons(Qt::LeftButton);
    setAcceptHoverEvents(true);
}

void dvd::UiOverlay::paint(
        QPainter* painter, QStyleOptionGraphicsItem const*, QWidget*)
{
    // In red, draw the buttons from the DvD
    painter->setPen(Qt::red);
    for (dvd::MenuButton button : m_buttons)
    {
        painter->drawRect( button.mapToScreen(m_size) );
    }

    // Draw the bounding box around the entire overlay
    painter->setPen( Qt::blue );
    painter->drawRect( QRectF( QPointF(0,0), m_size - QSizeF(1,1) ) );
}

QRectF dvd::UiOverlay::boundingRect() const
{
    return QRectF( QPointF(), m_size );
}

void dvd::UiOverlay::setSize(QSizeF const& size)
{
    m_size = size;
}

void dvd::UiOverlay::setButtons( QList<dvd::MenuButton> const& buttons)
{
    m_buttons = buttons;
}

void dvd::UiOverlay::cursorIsInactive()
{
    std::cout << "Cursor is inactive" << std::endl;
}

void dvd::UiOverlay::hoverMoveEvent(QGraphicsSceneHoverEvent* e)
{
    cursorIsActive();

    bool canClick(false);

    for ( dvd::MenuButton const& btn : m_buttons )
    {
        if ( btn.mapToScreen( boundingRect().size() ).contains(e->pos()) )
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

void dvd::UiOverlay::mousePressEvent(QGraphicsSceneMouseEvent* e)
{
    for ( dvd::MenuButton const& btn : m_buttons )
    {
        if ( btn.mapToScreen( boundingRect().size() ).contains(e->pos()) )
        {
            activate(btn);
            break;
        }
    }
}
