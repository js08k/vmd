#pragma once

#include "dvd/dvd.h"
#include <QWidget>
#include <functional>

namespace Ui { class UiOverlay; }
namespace gui { class UiOverlay; }
class QPushButton;

class gui::UiOverlay
        : public QWidget
{
    Q_OBJECT
public:
    UiOverlay(QWidget* parent = 0);
    ~UiOverlay();
    inline void setActivate(std::function<void(dvd::MenuButton)>);
    inline void setHighlight(std::function<void(dvd::MenuButton)>);
    inline void setCursorIsActive(std::function<void()>);

    void resize( QSize );
    void paintEvent( QPaintEvent* e );

    void setButtons( QList<dvd::MenuButton> const& );
    void cursorIsInactive();

    void setConnected( bool );
    void setPeerAddress( QString const& );
    void setHostAddress( QString const& );

signals:
    void clickedConnect() const;
    void loadMedia( QString const& ) const;
    void peerAddressChanged( QString const& ) const;
    void hostAddressChanged( QString const& ) const;

protected:
    void mouseMoveEvent( QMouseEvent* e );
    void mousePressEvent( QMouseEvent* e );

private slots:
    void menuToggled(bool);
    void ldvdClicked();
    void loadClicked();
    void peerAddrEditingFinished();
    void hostAddrEditingFinished();

private:
    std::function<void(dvd::MenuButton)> activate;
    std::function<void(dvd::MenuButton)> highlight;
    std::function<void()> cursorIsActive;

private:
    Ui::UiOverlay* ui;
    QPushButton* m_ldvd;
    QPushButton* m_load;
    QPushButton* m_menu;

    QList<dvd::MenuButton> m_buttons;
};

void gui::UiOverlay::setActivate(std::function<void(dvd::MenuButton)> fn)
{ activate = fn; }

void gui::UiOverlay::setHighlight(std::function<void(dvd::MenuButton)> fn)
{ highlight = fn; }

void gui::UiOverlay::setCursorIsActive(std::function<void()> fn)
{ cursorIsActive = fn; }
