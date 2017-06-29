#pragma once

#include "dvd/dvd_global.h"
#include <QObject>
#include <QList>
#include "dvd/menubutton.h"

class QTimer;

// Forward declaration of dvdnav_s & dvdnav_t
typedef struct dvdnav_s dvdnav_t;

namespace dvd
{
    enum MediaState
    {
        MediaNotAvailable,
        MediaLoading,
        MediaIdle,
        MediaReading
    };

    enum StreamAction
    {
        AppendStream,
        FlushStream
    };
}

namespace dvd { class MediaFrame; }

class DVDSHARED_EXPORT DvDContext
        : public QObject
{
    Q_OBJECT

public:
    DvDContext(QObject* parent = 0);
    virtual ~DvDContext();
    bool open( QString const& device );

signals:
    void title( QString const& ) const;
    void stream( dvd::MediaFrame const& frame ) const;
    void resolution( QSizeF const& ) const;

    // Signal for streaming data, when flush is set flush any previously
    // buffered data
    void stream( QByteArray const& data, dvd::StreamAction action ) const;
    void mediaStateChanged( dvd::MediaState ) const;
    void buttons( QList<MenuButton> const& ) const;

public slots:
    void highlight( MenuButton const& );
    void activate( MenuButton const& );
    void pause();
    void resume();

private slots:
    void loop();

private:
    void setMediaState( dvd::MediaState state );

private:
    dvdnav_t* m_handle;
    QString m_title;
    QTimer* m_loop;
    QList<MenuButton> m_buttons;

    dvd::MediaState m_mediaState;
    bool m_flush;
    QSizeF m_resolution;
};
