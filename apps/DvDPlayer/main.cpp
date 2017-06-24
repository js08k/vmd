#include "DvDPlayer.h"
#include <QApplication>
#include <QFile>
#include "MediaScreen.h"
#include "iostream"
#include "dvd/dvdcontext.h"
#include "dvd/mpegcontext.h"
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QBuffer>
#include "dvd/streambuffer.h"
#include <QTimer>
#include "qmediaplayerspy.h"
#include "dvd/VideoWidget.h"

extern "C" {
#include "libavcodec/avcodec.h"
}

int main(int argc, char *argv[])
{
    avcodec_register_all();
    QApplication a(argc, argv);

    std::cout << argv[1] << std::endl;
    QFile file( argv[1] );

    if ( file.exists() && file.open(QIODevice::ReadOnly) )
    {

        QMediaPlayer player;
        QMediaPlayerSpy spy(&player);

        VideoWidget widget;
        widget.resize( 720, 480 );

        DvDContext dvdcontext;
        dvdcontext.open( "/dev/dvd" );
        StreamBuffer buffer;

        QObject::connect( &buffer, SIGNAL(pauseReadStream()), &dvdcontext, SLOT(pause()) );
        QObject::connect( &buffer, SIGNAL(resumeReadStream()), &dvdcontext, SLOT(resume()) );
        QObject::connect( &dvdcontext, SIGNAL(stream(QByteArray,dvd::StreamAction)), &buffer, SLOT(stream(QByteArray,dvd::StreamAction)) );
        QObject::connect( &dvdcontext, SIGNAL(buttons(QList<MenuButton>)),
                          &widget, SLOT(buttons(QList<MenuButton>)) );
        QObject::connect(&widget, SIGNAL(highlight(MenuButton)),
                         &dvdcontext, SLOT(highlight(MenuButton)) );
        QObject::connect(&widget, SIGNAL(activate(MenuButton)),
                         &dvdcontext, SLOT(activate(MenuButton)) );

        player.setMedia( QMediaContent(), &buffer);
        player.setVideoOutput(&widget);

//        player.play();

        QTimer::singleShot( 5000, &player, SLOT(play()) );

        widget.show();

        return a.exec();
    }
    else
    {
        std::cout << "Could not find a useable media file" << std::endl;
        return 1;
    }
}
