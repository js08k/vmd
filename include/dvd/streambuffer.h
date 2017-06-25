#ifndef STREAMBUFFER_H
#define STREAMBUFFER_H

#include <QBuffer>
#include "dvd/dvdcontext.h"

class QTimer;

class StreamBuffer
        : public QIODevice
{
    Q_OBJECT
public:
    StreamBuffer(QObject* parent = 0);
    StreamBuffer(QByteArray const& other, QObject* parent = 0);

    virtual ~StreamBuffer();

    virtual bool isSequential() const
    { return true; }

    virtual qint64 bytesAvailable() const;

    virtual void close();
    virtual bool open(OpenMode flags);
    virtual qint64 pos() const;
    virtual bool seek(qint64 pos);
    virtual qint64 size() const;

public slots:
    void stream(QByteArray const&,dvd::StreamAction);

signals:
    void ready() const;
    void pauseReadStream() const;
    void resumeReadStream() const;

protected:
    virtual qint64 readData(char* data, qint64 maxSize);
    virtual qint64 writeData(char const*, qint64);

private:
    int const m_minbuffered;
    int const m_maxbuffered;
    QByteArray m_data;
    bool m_ready;
};

#endif // STREAMBUFFER_H
