#ifndef MPEGCONTEXT_H
#define MPEGCONTEXT_H

#include <QObject>
#include <QFile>
#include <QByteArray>

class QTimer;

class MPEGContext
        : public QObject
{
    Q_OBJECT
public:
    MPEGContext(QObject* parent=0);
    virtual ~MPEGContext();
    bool open( QString const& file );

signals:
    void media( QByteArray const& ) const;

public slots:
    void pauseRead();
    void resumeRead();

private slots:
    void loop();

private:
    QTimer* m_loop;
    QFile m_file;
    QByteArray m_buffer;
};

#endif // MPEG2CONTEXT_H
