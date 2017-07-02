/*
@startuml

class MenuButton {
+MenuButton(int)
+operator==(MenuButton) : bool
+isNull() : bool
+mapToScreen(QSizeF) const : QRectF
+setResolution(QSizeF)
+resolution() const : QRectF
+setButtonRect(QRectF)
+buttonRect() const : QRectF
-QUuid m_id
-int m_index
-QSizeF m_resolution
-QRectF m_button
}

@enduml
*/
#include "dvd/menubutton.h"

// Qt Includes

dvd::MenuButton::MenuButton( int index )
    : m_id(QUuid::createUuid())
    , m_index(index)
{
}

dvd::MenuButton::MenuButton( MenuButton const& other )
    : m_id( other.m_id )
    , m_index( other.m_index )
    , m_resolution( other.m_resolution )
    , m_button( other.m_button )
{

}

dvd::MenuButton::~MenuButton()
{

}

bool dvd::MenuButton::operator==( MenuButton const& other) const
{
    return m_id == other.m_id;
}

bool dvd::MenuButton::isNull() const
{
    return bool(m_index == -1);
}

int dvd::MenuButton::index() const
{
    return m_index;
}

QRectF dvd::MenuButton::mapToScreen(QSizeF const& screen) const
{
    qreal const xscale( screen.width() / m_resolution.width() );
    qreal const yscale( screen.height() / m_resolution.height() );

    return QRectF( m_button.left()*xscale, m_button.top()*yscale,
                   m_button.width()*xscale, m_button.height()*yscale );
}

void dvd::MenuButton::setResolution(const QSizeF &resolution)
{
    m_resolution = resolution;
}

QSizeF dvd::MenuButton::resolution() const
{
    return m_resolution;
}

void dvd::MenuButton::setButtonRect(QRectF const& button)
{
    m_button = button;
}

QRectF dvd::MenuButton::buttonRect() const
{
    return m_button;
}

bool dvd::MenuButton::update( MenuButton const& other )
{
    bool updated(false);

    if ( other.m_resolution.toSize() != m_resolution.toSize() )
    {
        m_resolution = other.m_resolution;
        updated = true;
    }

    if ( other.m_button.toRect() != m_button.toRect() )
    {
        m_button = other.m_button;
        updated = true;
    }

    return updated;
}
