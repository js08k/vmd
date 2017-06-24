#ifndef QMEDIAPLAYERSPY_H
#define QMEDIAPLAYERSPY_H

#include <QObject>
#include <QMediaPlayer>

class QMediaPlayerSpy : public QObject
{
    Q_OBJECT
public:
    explicit QMediaPlayerSpy(QMediaPlayer *parent);

private slots:
    void audioAvailableChanged(bool available);
    void audioRoleChanged(QAudio::Role role);
    void bufferStatusChanged(int percentFilled);
    void currentMediaChanged(const QMediaContent &media);
    void durationChanged(qint64 duration);
    void error(QMediaPlayer::Error error);
    void mediaChanged(const QMediaContent &media);
    void mediaStatusChanged(QMediaPlayer::MediaStatus status);
    void mutedChanged(bool muted);
    void networkConfigurationChanged(const QNetworkConfiguration &configuration);
    void playbackRateChanged(qreal rate);
    void positionChanged(qint64 position);
    void seekableChanged(bool seekable);
    void stateChanged(QMediaPlayer::State state);
    void videoAvailableChanged(bool videoAvailable);
    void volumeChanged(int volume);

};

#endif // QMEDIAPLAYERSPY_H
