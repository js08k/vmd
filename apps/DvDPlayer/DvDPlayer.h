#ifndef DVDPLAYER_H
#define DVDPLAYER_H

#include <QWidget>

namespace Ui {
class DvDPlayer;
}

class DvDPlayer : public QWidget
{
    Q_OBJECT

public:
    explicit DvDPlayer(QWidget *parent = 0);
    ~DvDPlayer();

private:
    Ui::DvDPlayer *ui;
};

#endif // DVDPLAYER_H
