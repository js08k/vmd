#include "qmediaplayerspy.h"
#include <iostream>

QMediaPlayerSpy::QMediaPlayerSpy(QMediaPlayer *parent)
    : QObject(parent)
{
    connect( parent, SIGNAL(audioAvailableChanged(bool)),
             this, SLOT(audioAvailableChanged(bool)) );
    connect( parent, SIGNAL(audioRoleChanged(QAudio::Role)),
             this, SLOT(audioRoleChanged(QAudio::Role)));
    connect( parent, SIGNAL(bufferStatusChanged(int)),
             this, SLOT(bufferStatusChanged(int)) );
    connect( parent, SIGNAL(currentMediaChanged(QMediaContent)),
             this, SLOT(currentMediaChanged(QMediaContent)) );
    connect( parent, SIGNAL(durationChanged(qint64)),
             this, SLOT(durationChanged(qint64)) );
    connect( parent, SIGNAL(error(QMediaPlayer::Error)),
             this, SLOT(error(QMediaPlayer::Error)) );
    connect( parent, SIGNAL(mediaChanged(QMediaContent)),
             this, SLOT(mediaChanged(QMediaContent)) );
    connect( parent, SIGNAL(networkConfigurationChanged(QNetworkConfiguration)),
             this, SLOT(networkConfigurationChanged(QNetworkConfiguration)) );
    connect( parent, SIGNAL(playbackRateChanged(qreal)),
             this, SLOT(playbackRateChanged(qreal)) );
    connect( parent, SIGNAL(positionChanged(qint64)),
             this, SLOT(positionChanged(qint64)) );
    connect( parent, SIGNAL(seekableChanged(bool)),
             this, SLOT(seekableChanged(bool)) );
    connect( parent, SIGNAL(stateChanged(QMediaPlayer::State)),
             this, SLOT(stateChanged(QMediaPlayer::State)) );
    connect( parent, SIGNAL(videoAvailableChanged(bool)),
             this, SLOT(videoAvailableChanged(bool)) );
    connect( parent, SIGNAL(volumeChanged(int)),
             this, SLOT(volumeChanged(int)) );
}

void QMediaPlayerSpy::audioAvailableChanged(bool available)
{
    std::cout << "Spy: audioAvailableChanged("
              << (available?"true":"false")<< ")" << std::endl;
}

void QMediaPlayerSpy::audioRoleChanged(QAudio::Role role)
{
    std::cout << "Spy: audioRoleChanged(" << int(role) << ")" << std::endl;
}

void QMediaPlayerSpy::bufferStatusChanged(int percentFilled)
{
    std::cout << "Spy: bufferStatusChanged(" << int(percentFilled) << ")" << std::endl;
}

void QMediaPlayerSpy::currentMediaChanged(const QMediaContent &media)
{
    Q_UNUSED(media);
    std::cout << "Spy: currentMediaChanged()" << std::endl;
}

void QMediaPlayerSpy::durationChanged(qint64 duration)
{
    std::cout << "Spy: durationChanged(" << int(duration) << ")" << std::endl;
}

void QMediaPlayerSpy::error(QMediaPlayer::Error error)
{
    std::cout << "Spy: error(" << int(error) << ")" << std::endl;
}

void QMediaPlayerSpy::mediaChanged(const QMediaContent &media)
{
    Q_UNUSED(media);
    std::cout << "Spy: mediaChanged()" << std::endl;
}

void QMediaPlayerSpy::mediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    std::cout << "Spy: mediaStatusChanged(" << int(status) << ")" << std::endl;
}

void QMediaPlayerSpy::mutedChanged(bool muted)
{
    std::cout << "Spy: mutedChanged("
              << (muted?"true":"false") << ")" << std::endl;
}

void QMediaPlayerSpy::networkConfigurationChanged(
        const QNetworkConfiguration &configuration)
{
    Q_UNUSED(configuration);
    std::cout << "Spy: networkConfigurationChanged()" << std::endl;
}

void QMediaPlayerSpy::playbackRateChanged(qreal rate)
{
    std::cout << "Spy: playbackRateChanged(" << rate << ")" << std::endl;
}

void QMediaPlayerSpy::positionChanged(qint64 position)
{
    std::cout << "Spy: positionChanged(" << position << ")" << std::endl;
}

void QMediaPlayerSpy::seekableChanged(bool seekable)
{
    std::cout << "Spy: seekableChanged("
              << (seekable?"true":"false") << ")" << std::endl;
}

void QMediaPlayerSpy::stateChanged(QMediaPlayer::State state)
{
    std::cout << "Spy: stateChanged(" << state << ")" << std::endl;
}

void QMediaPlayerSpy::videoAvailableChanged(bool videoAvailable)
{
    std::cout << "Spy: videoAvailableChanged("
              << (videoAvailable?"true":"false") << ")" << std::endl;
}

void QMediaPlayerSpy::volumeChanged(int volume)
{
    std::cout << "Spy: volumeChanged(" << volume << ")" << std::endl;
}
