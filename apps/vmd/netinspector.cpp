#include "netinspector.h"
#include <QTimer>
#include <QUuid>

#include <iostream>

NetInspector::NetInspector(QObject *parent)
    : QObject(parent)
{
}

NetInspector::~NetInspector()
{

}

void NetInspector::measure()
{
    QTimer::singleShot( 1000, this, SLOT(measure()) );

    // Generate new data for a report
    QUuid const report( QUuid::createUuid() );
    QByteArray const bytes( report.toRfc4122() );

    // Build the report and transmit
    gtqt::DataPackage<gtqt::NetPing> msg;
    msg.data()->set_type( proto::Request );
    msg.data()->set_report( bytes.data(), bytes.length() );
    emit publish(msg);

    // Create the QTime and start
    m_reports[report].start();
}

void NetInspector::cleanup()
{
    // Max tolerable delay, 15 seconds
    int const delaymax(15000);

    QTimer::singleShot( delaymax, this, SLOT(cleanup()) );

    int const delay( m_delay < delaymax ? m_delay : delaymax );

    QHash<QUuid,QTime>::iterator i( m_reports.begin() );
    while ( i != m_reports.end() )
    {
        if ( (*i).elapsed() < 2*delay )
        { ++i; }
        else
        {
            std::cout << "Packet timeout" << std::endl;
            m_average.push_front( (*i).elapsed() );
            i = m_reports.erase(i);
            average();
        }
    }
}

void NetInspector::receive( gtqt::DataPackage<gtqt::NetPing> msg )
{
    if ( msg->type() == proto::Request )
    {
        msg.data()->set_type( proto::Reply );
        emit publish(msg);
    }
    else
    {
        QByteArray const bytes(msg->report().c_str(), msg->report().length());
        QUuid const report( QUuid::fromRfc4122(bytes) );

        QHash<QUuid,QTime>::iterator i( m_reports.find(report) );
        if ( i != m_reports.end() )
        {
            std::cout << "Packet reply " << (*i).elapsed() << std::endl;

            // Insert the result to the Average
            m_average.push_front((*i).elapsed());

            // Perform the average calculation
            average();

            // Erase the report
            m_reports.erase(i);
        }
    }
}

void NetInspector::average()
{
    if ( m_average.length() > 30 )
    { m_average.pop_back(); }

    // Calculate the average delay over the last up to 30 measurments
    int average(0);
    foreach ( int value, m_average )
    { average += value; }

    // Save the calculated value
    m_delay = average / m_average.length();

    std::cout << "Average: " << m_delay << std::endl;
}
