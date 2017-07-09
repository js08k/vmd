#include "vmd.h"
#include <QApplication>
#include <QSettings>
#include "dvd/mediainput.h"
#include "guiUtil/VideoWidget.h"
#include <signal.h>

#include <iostream>

int main(int argc, char *argv[])
{
    auto handler = [](int) { QCoreApplication::quit(); };
    signal( SIGQUIT, handler );
    signal( SIGTERM, handler );
    signal( SIGINT, handler );
    signal( SIGHUP, handler );

    QApplication app(argc, argv);

    // Configure Applicaton information relevant for using QSettings
    QCoreApplication::setOrganizationName("OpenSource");
    QCoreApplication::setApplicationName("vmd");

#if 0
    VMD mainwindow;
    mainwindow.show();
#else
    gui::VideoWidget vw;
    vw.show();
#endif

    return app.exec();
}
