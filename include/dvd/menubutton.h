#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include <QRectF>
#include <QUuid>

class MenuButton
{
public:
    MenuButton( int index = -1 );
    MenuButton( MenuButton const& other );
    virtual ~MenuButton();

    // Compare Operator
    bool operator==( MenuButton const& other) const;

    // isNull check
    bool isNull() const;

    // Returns the index the MenuButton was created with
    int index() const;

    // Map the button position given a screen resolution
    QRectF mapToScreen(QSizeF const& screen) const;

    // Get and set resolution of the screen
    void setResolution(QSizeF const&);
    QSizeF resolution() const;

    // Set the button Rectangle in the resolution domain
    void setButtonRect(QRectF const&);
    QRectF buttonRect() const;

    bool update( MenuButton const& other );

private:
    QUuid m_id;
    int m_index;

    QSizeF m_resolution;
    QRectF m_button;

};

#endif // MENUBUTTON_H

