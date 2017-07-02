#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QVideoWidget>
#include <QPoint>
#include <QTime>
#include "dvd/menubutton.h"

class QMouseEvent;
class QTimer;

class VideoWidget
        : public QVideoWidget
{
    Q_OBJECT

public:
    VideoWidget( QWidget* parent = 0 );
    virtual ~VideoWidget();

signals:
    void highlight( dvd::MenuButton const& ) const;
    void activate( dvd::MenuButton const& ) const;

public slots:
    void buttons( QList<dvd::MenuButton> const& );

protected:
    virtual void mouseMoveEvent( QMouseEvent* event );
    virtual void mousePressEvent( QMouseEvent* event );

private slots:
    void cursorSpy();

private:
    QPoint m_lastCursorPosition;
    QTime m_lastCursorMovement;
    QList<dvd::MenuButton> m_buttons;
};

#endif // VIDEOWIDGET_H
