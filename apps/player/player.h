#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class VlcInstance;
class VlcMedia;
class VlcMediaPlayer;

namespace Ui { class Player; }

class Player
        : public QWidget
{
    Q_OBJECT

public:
    explicit Player(QWidget *parent = 0);
    ~Player();

private slots:
    void clickedPushButtonLoad();

private:
    Ui::Player *ui;

    VlcInstance* m_instance;
    VlcMedia* m_media;
    VlcMediaPlayer* m_player;
};

#endif // MAINWINDOW_H
