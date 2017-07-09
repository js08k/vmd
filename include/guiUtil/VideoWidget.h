#pragma once

#include "dvd/menubutton.h"
#include <QVideoWidget>

namespace gui { class VideoWidget; }
namespace gui { class UiOverlay; }

class gui::VideoWidget
        : public QVideoWidget
{
    Q_OBJECT
public:
    explicit VideoWidget(QWidget* parent = 0);
    ~VideoWidget();
    inline QMediaBindableInterface* device() const;

signals:
    void highlight( dvd::MenuButton const& ) const;
    void activate( dvd::MenuButton const& ) const;
    void clickedConnect() const;
    void loadMedia( QString const& ) const;
    void peerAddressChanged( QString const& ) const;
    void hostAddressChanged( QString const& ) const;

public slots:
    void resolution( QSizeF const& );
    void buttons( QList<dvd::MenuButton> const& );

protected:
    void closeEvent(QCloseEvent* e);
    void moveEvent(QMoveEvent* e);
    void hideEvent(QHideEvent* e);
    void showEvent(QShowEvent* e);
    void resizeEvent(QResizeEvent* e);
    void focusOutEvent(QFocusEvent* e);
    void focusInEvent(QFocusEvent* e);

private slots:
    void cursorIsInactive();

private:
    gui::UiOverlay* m_overlay;
    QTimer* m_cursorActiveTimer;
};

QMediaBindableInterface* gui::VideoWidget::device() const
{
    return const_cast<gui::VideoWidget*>(this);
}
