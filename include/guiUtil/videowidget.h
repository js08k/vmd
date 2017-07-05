#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include "dvd/menubutton.h"
#include <QGraphicsView>

class QGraphicsVideoItem;
namespace dvd { class UiOverlay; }
namespace dvd { class VideoWidget; }

class dvd::VideoWidget
        : public QGraphicsView
{
    Q_OBJECT
public:
    VideoWidget(QWidget *parent = 0);
    ~VideoWidget();
    inline QGraphicsVideoItem* device() const;

signals:
    void highlight( dvd::MenuButton const& ) const;
    void activate( dvd::MenuButton const& ) const;

public slots:
    void resolution( QSizeF const& );
    void buttons( QList<dvd::MenuButton> const& );

private slots:
    void cursorIsInactive();

private:
    QGraphicsVideoItem* m_output;
    dvd::UiOverlay* m_overlay;
    QTimer* m_cursorActiveTimer;
};

QGraphicsVideoItem* dvd::VideoWidget::device() const
{ return m_output; }

#endif // VIDEOWIDGET_H
