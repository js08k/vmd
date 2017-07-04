#pragma once

#include "dvd/dvd.h"

#include "dvd/mediasender.h"

#include <QObject>
#include <QList>
#include "dvd/menubutton.h"
#include <iostream>

class QTimer;

// Forward declaration of dvdnav_s & dvdnav_t
typedef struct dvdnav_s dvdnav_t;

class DVDSHARED_EXPORT dvd::DvDContext
        : public dvd::MediaSender
{
    Q_OBJECT

public:
    DvDContext(QObject* parent = 0);
    virtual ~DvDContext();
    void open( QString const& device );

signals:
    void buttons( QList<dvd::MenuButton> const& ) const;

public slots:
    void menu();
    void highlight( dvd::MenuButton const& );
    void activate( dvd::MenuButton const& );
    void pauseStream();
    void resumeStream();

private slots:
    void loop();
    void setReadStart();

private:
    dvd::MediaState m_state;

    dvdnav_t* m_handle;
    QString m_title;
    QTimer* m_loop;
    QList<MenuButton> m_buttons;

    bool m_flush;
    QSizeF m_resolution;
};
