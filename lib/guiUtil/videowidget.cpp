#include "guiUtil/videowidget.h"
#include "guiUtil/uioverlay.h"

#include <QGraphicsScene>
#include <QGraphicsVideoItem>

dvd::VideoWidget::VideoWidget(QWidget *parent)
    : QGraphicsView(parent)
    , m_output( new QGraphicsVideoItem )
    , m_overlay( new dvd::UiOverlay )
{
    // Create and set the scene
    setScene( new QGraphicsScene(this) );
    scene()->setSceneRect( rect() );

    // Disable scroll bars
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Add the video item to the scene
    m_output->setAspectRatioMode(Qt::IgnoreAspectRatio);
    scene()->addItem(m_output);

    // Finish construction/initialization of the overlay, and add to the scene
    m_overlay->setActivate([&](dvd::MenuButton btn){ emit activate(btn); });
    m_overlay->setHighlight([&](dvd::MenuButton btn){ emit highlight(btn); });
    m_overlay->setSize( size() );
    scene()->addItem(m_overlay);
}

dvd::VideoWidget::~VideoWidget()
{

}

void dvd::VideoWidget::resolution( QSizeF const& size )
{
    // Calculate a Rectangle to represent the resolution with a 1 pixel border
    QRectF const rect( QPointF(), size+QSizeF(2,2) );

    // Set the scene to this calculated rectangle
    scene()->setSceneRect( rect );

    // Set the view to the same rectangle
    setMinimumSize( rect.size().toSize() );

    // Set the video output to the recommended resolution
    m_output->setSize( size );

    // Set the uioverlay to the same resolution as the video output
    m_overlay->setSize( size );

    // Ensure the view centers correctly around the video output object
    ensureVisible( m_overlay->boundingRect(), 0, 0);
}

void dvd::VideoWidget::buttons( QList<dvd::MenuButton> const& btns)
{
    m_overlay->setButtons(btns);
}
