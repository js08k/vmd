#include "DvDPlayer.h"
#include "ui_DvDPlayer.h"

DvDPlayer::DvDPlayer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DvDPlayer)
{
    ui->setupUi(this);
}

DvDPlayer::~DvDPlayer()
{
    delete ui;
}
