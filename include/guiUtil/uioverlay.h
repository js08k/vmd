#pragma once

#include "dvd/dvd.h"
#include <functional>
#include <QGraphicsItem>

namespace dvd { class UiOverlay; }

class dvd::UiOverlay
        : public QGraphicsItem
{
public:
    UiOverlay();
    inline void setActivate(std::function<void(dvd::MenuButton)>);
    inline void setHighlight(std::function<void(dvd::MenuButton)>);
    inline void setCursorIsActive(std::function<void()>);

    void paint(QPainter*, QStyleOptionGraphicsItem const*, QWidget*);
    QRectF boundingRect() const;

    void setSize( QSizeF const& );
    void setButtons( QList<dvd::MenuButton> const& );
    void cursorIsInactive();

protected:
    void hoverMoveEvent(QGraphicsSceneHoverEvent* event);
    void mousePressEvent(QGraphicsSceneMouseEvent* event);

private:
    std::function<void(dvd::MenuButton)> activate;
    std::function<void(dvd::MenuButton)> highlight;
    std::function<void()> cursorIsActive;

private:
    QSizeF m_size;
    QList<dvd::MenuButton> m_buttons;
};

void dvd::UiOverlay::setActivate(std::function<void(dvd::MenuButton)> fn)
{ activate = fn; }

void dvd::UiOverlay::setHighlight(std::function<void(dvd::MenuButton)> fn)
{ highlight = fn; }

void dvd::UiOverlay::setCursorIsActive(std::function<void()> fn)
{ cursorIsActive = fn; }
