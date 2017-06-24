#include "MediaScreen.h"

// Std C++ Inlcudes
#include <iostream>

// Qt Includes
#include <QTimer>
#include <QPainter>
#include <QCoreApplication>

// Video Decoding
extern "C"
{
#include <math.h>
#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavutil/channel_layout.h>
#include <libavutil/common.h>
#include <libavutil/imgutils.h>
#include <libavutil/mathematics.h>
#include <libavutil/samplefmt.h>
#define INBUF_SIZE 4096
#define AUDIO_INBUF_SIZE 20480
#define AUDIO_REFILL_THRESH 4096
}

MediaScreen::MediaScreen(QWidget* parent)
    : QWidget(parent)
    , m_imagemax(50)
    , m_buffermax(1024*1024*1024)   // 1mB
    , m_loop( new QTimer(this) )
    , m_frameUpdate( new QTimer(this) )
    , m_codec(avcodec_find_decoder(AV_CODEC_ID_MPEG2VIDEO))
    , m_context(0)
{
    if ( !m_codec )
    {
        std::cerr << "Codec not found" << std::endl;
        QCoreApplication::exit(1);
    }
    else
    {
        m_context = avcodec_alloc_context3(m_codec);
        if ( !m_context )
        {
            std::cerr << "Could not allocate video codec context" << std::endl;
            QCoreApplication::exit(1);
        }
        else
        {
            if ( m_codec->capabilities & AV_CODEC_CAP_TRUNCATED )
                m_context->flags |= AV_CODEC_FLAG_TRUNCATED;

            if ( avcodec_open2(m_context,m_codec,NULL) < 0 )
            {
                std::cerr << "Could not open codec" << std::endl;
                QCoreApplication::exit(1);
            }
        }
    }

    // Frame processing loop
    m_loop->setInterval(10);
    connect( m_loop, SIGNAL(timeout()), this, SLOT(loop()) );

    // 25 fps
    m_frameUpdate->setInterval(40);
    connect( m_frameUpdate, SIGNAL(timeout()), this, SLOT(frameUpdate()) );
    m_frameUpdate->start();
}

MediaScreen::~MediaScreen()
{

}

void MediaScreen::media( QByteArray const& data )
{
    // Buffer the new data
    m_databuffer += data;

    if ( m_databuffer.length() > m_buffermax )
    {
        std::cout << "Stopping data, we have enough" << std::endl;
        // We have plenty of buffered data, tell the reader to stop
        emit pauseReadLoop();
    }

    if ( !m_loop->isActive() )
    {
        std::cout << "Starting frame loop, new data" << std::endl;

        // processing frame loop is paused, begin processing
        m_loop->start();
    }
}

void MediaScreen::frameUpdate()
{
    // Do nothing if the frame buffer is empty
    if ( m_framebuffer.isEmpty() )
        return;

    // Take the first frame, setting it to the current frame
    m_frame = m_framebuffer.takeFirst();

    // Resize this widget to the size of the frame
    if ( m_frame.size() != size() )
        resize( m_frame.size() );

    // Call update to perform the draw
    update();

    // Restart the processing loop if we are under our imagemin
    if ( !m_loop->isActive() && m_framebuffer.isEmpty() )
    {
        std::cout << "Starting frame loop" << std::endl;
        m_loop->start();
    }
}

void MediaScreen::loop()
{
    if ( m_databuffer.isEmpty() )
    {
        std::cout << "Data consumed, requesting data" << std::endl;
        emit resumeReadLoop();
        m_loop->stop();
        return;
    }
    else if ( m_framebuffer.length() > m_imagemax )
    {
        std::cout << "Stopping frame loop" << std::endl;

        // We have exceeded our buffered image count, stop the loop and wait
        // fro the frames to be consumed
        m_loop->stop();
        return;
    }

    AVPacket avpkt;
    av_init_packet(&avpkt);

    AVFrame* frame( av_frame_alloc() );
    if ( !frame )
    {
        std::cerr << "Could not allocate video frame" << std::endl;
        QCoreApplication::exit(1);
        return;
    }

    // Read the data from our buffer into the example buffer
    avpkt.size = m_databuffer.length();
    avpkt.data = reinterpret_cast<uint8_t*>(m_databuffer.data());

    int len, got_frame;
    len = avcodec_decode_video2( m_context, frame, &got_frame, &avpkt);

    if ( len < 0 )
    {
        m_framebuffer.clear();
        std::cerr << "Error decoding frame" << std::endl;
        return;
    }
    else
    {
        m_databuffer.remove(0, len);
    }

    if ( got_frame )
    {
        static const QString _header( "P5\n%1 %2\n255\n" );
        QString const header( _header.arg(frame->width).arg(frame->height) );

        QByteArray image( header.toUtf8() );
        for (int i = 0; i < frame->height; i++)
            image += QByteArray((char*)(frame->data[0]+i*frame->linesize[0]), frame->width);

        m_framebuffer.append( QImage::fromData(image) );
    }
}

void MediaScreen::paintEvent(QPaintEvent* event)
{

    QWidget::paintEvent(event);

    if ( m_frame.isNull() ) { return; }
    QPainter painter(this);
    painter.drawImage( m_frame.rect(), m_frame );
}
