#ifndef NETINSPECTOR_H
#define NETINSPECTOR_H

#include <QObject>
#include <QTime>

#include "message.h"
#include "datapackage.h"

class QTimer;

class NetInspector
        : public QObject
{
    Q_OBJECT
public:
    explicit NetInspector(QObject *parent = nullptr);
    virtual ~NetInspector();

signals:
    void publish( gtqt::DataPackage<gtqt::NetPing> const& ) const;

public slots:
    void receive( gtqt::DataPackage<gtqt::NetPing> );
    void measure();
    void cleanup();

private:
    void average();

private:
    int m_delay;
    QList<quint32> m_average;
    QHash<QUuid,QTime> m_reports;
};

#endif // NETINSPECTOR_H
