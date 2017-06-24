#ifndef MEDIASCREEN_H
#define MEDIASCREEN_H

#include <QWidget>
#include <QImage>
#include <QList>

class QTimer;
struct AVCodec;
struct AVCodecContext;

class MediaScreen
        : public QWidget
{
    Q_OBJECT
public:
    MediaScreen( QWidget* parent = 0 );
    virtual ~MediaScreen();
    virtual void paintEvent(QPaintEvent* event);

signals:
    void pauseReadLoop() const;
    void resumeReadLoop() const;

public slots:
    void media( QByteArray const& );

private slots:
    void loop();
    void frameUpdate();

private:
    int const m_imagemax;
    int const m_buffermax;

    QTimer* m_loop;
    QTimer* m_frameUpdate;

    QImage m_frame;
    QList<QImage> m_framebuffer;
    QByteArray m_databuffer;

    AVCodec* m_codec;
    AVCodecContext* m_context;
};

#endif // MEDIASCREEN_H
