#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QVideoWidget>
#include <QPoint>
#include <QTime>
#include "dvd/menubutton.h"
#include <QGraphicsView>
#include <QGraphicsVideoItem>

class QMouseEvent;
class QTimer;
namespace dvd { class UiOverlay; }

class dvd::UiOverlay
        : public QGraphicsItem
{
public:
    UiOverlay() { }
    void paint(QPainter*, QStyleOptionGraphicsItem const*, QWidget*);
    QRectF boundingRect() const;
    void setRect( QRect );
    void buttons( QList<dvd::MenuButton> const& );

private:
    QRectF m_rect;
    QList<dvd::MenuButton> m_buttons;
};

class VideoWidget
        : public QGraphicsView
{
    Q_OBJECT

public:
    VideoWidget( QWidget* parent = 0);
    inline QGraphicsVideoItem* device() const;

signals:
    void highlight( dvd::MenuButton const& ) const;
    void activate( dvd::MenuButton const& ) const;

public slots:
    void buttons( QList<dvd::MenuButton> const& );

protected:
    void resizeEvent(QResizeEvent* e);
    virtual void mouseMoveEvent( QMouseEvent* event );
    virtual void mousePressEvent( QMouseEvent* event );

private slots:
    void cursorSpy();
    void videoNativeSizeChanged( QSizeF );

private:
    QGraphicsVideoItem* m_vOutput;
    dvd::UiOverlay* m_uioverlay;

    QPoint m_lastCursorPosition;
    QTime m_lastCursorMovement;
    QList<dvd::MenuButton> m_buttons;
};

QGraphicsVideoItem* VideoWidget::device() const
{ return m_vOutput; }

#endif // VIDEOWIDGET_H
