#include "dvd/menubutton.h"

MenuButton::MenuButton( int index )
    : m_id(QUuid::createUuid())
    , m_index(index)
{

}

MenuButton::MenuButton( MenuButton const& other )
    : m_id( other.m_id )
    , m_index( other.m_index )
    , m_resolution( other.m_resolution )
    , m_button( other.m_button )
{

}

MenuButton::~MenuButton()
{

}

bool MenuButton::operator==( MenuButton const& other) const
{
    return m_id == other.m_id;
}

bool MenuButton::isNull() const
{
    return bool(m_index == -1);
}

int MenuButton::index() const
{
    return m_index;
}

QRectF MenuButton::mapToScreen(QSizeF const& screen) const
{
    qreal const xscale( screen.width() / m_resolution.width() );
    qreal const yscale( screen.height() / m_resolution.height() );

    return QRectF( m_button.left()*xscale, m_button.top()*yscale,
                   m_button.width()*xscale, m_button.height()*yscale );
}

void MenuButton::setResolution(const QSizeF &resolution)
{
    m_resolution = resolution;
}

QSizeF MenuButton::resolution() const
{
    return m_resolution;
}

void MenuButton::setButtonRect(QRectF const& button)
{
    m_button = button;
}

QRectF MenuButton::buttonRect() const
{
    return m_button;
}

bool MenuButton::update( MenuButton const& other )
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
