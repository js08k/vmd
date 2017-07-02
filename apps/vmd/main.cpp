#include "vmd.h"
#include <QApplication>
#include <QSettings>
#include "dvd/mediainput.h"
#include <signal.h>

#include <iostream>

int main(int argc, char *argv[])
{
    auto handler = [](int) { QCoreApplication::quit(); };
    signal( SIGQUIT, handler );
    signal( SIGTERM, handler );
    signal( SIGINT, handler );
    signal( SIGHUP, handler );

    dvd::MediaInput input;
    bool didLoad( input.initialize( "peer://192.168.1.23:2049/EASY_A" ) );
//    bool didLoad( input.initialize( "dvd:///dev/sr0" ) );
//    bool didLoad( input.initialize( "dvd:///home/josh/Videos/EASY_A.iso" ) );
//    bool didLoad( input.initialize( "file:///home/josh/Videos/natalie.mp4" ) );
    if ( didLoad )
        std::cout << "Loaded " << input.name().toStdString() << std::endl;
    else
        std::cout << "Failed dvd::MediaInput::setMedia()" << std::endl;

    QApplication app(argc, argv);

    // Configure Applicaton information relevant for using QSettings
    QCoreApplication::setOrganizationName("OpenSource");
    QCoreApplication::setApplicationName("vmd");

    VMD mainwindow;
    mainwindow.show();

    return app.exec();
}
