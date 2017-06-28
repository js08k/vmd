#include "vmd.h"
#include <QApplication>
#include <QSettings>
#include "dvd/mediainput.h"
#include <signal.h>

#include <iostream>

void shutdown(int)
{
    QCoreApplication::quit();
}

int main(int argc, char *argv[])
{
    signal( SIGQUIT, shutdown );
    signal( SIGTERM, shutdown );
    signal( SIGINT, shutdown );
    signal( SIGHUP, shutdown );
    dvd::MediaInput input( "dvd:///dev/sr0" );


    QApplication app(argc, argv);

    // Configure Applicaton information relevant for using QSettings
    QCoreApplication::setOrganizationName("OpenSource");
    QCoreApplication::setApplicationName("vmd");

    VMD mainwindow;
    mainwindow.show();

    return app.exec();
}
