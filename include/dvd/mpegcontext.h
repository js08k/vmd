#ifndef MPEGCONTEXT_H
#define MPEGCONTEXT_H

#include "dvd/dvd.h"

#include "dvd/mediasender.h"

#include <QObject>
#include <QFile>
#include <QByteArray>

class QTimer;

class DVDSHARED_EXPORT dvd::MPEGContext
        : public dvd::MediaSender
{
    Q_OBJECT
public:
    MPEGContext(QObject* parent=0);
    virtual ~MPEGContext();
    void open( QString const& file );

public slots:
    void pauseStream();
    void resumeStream();

    // Not implementing DvD Controlls for MPEGContext
    void menu() { }
    void highlight(dvd::MenuButton const&) { }
    void activate(dvd::MenuButton const&) { }

private slots:
    void loop();

private:
    QTimer* m_loop;
    QFile m_file;
    QByteArray m_buffer;
};

#endif // MPEG2CONTEXT_H

