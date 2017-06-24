#include "player.h"
#include "ui_player.h"

#include <VLCQtCore/Common.h>
#include <VLCQtCore/Instance.h>
#include <VLCQtCore/Media.h>
#include <VLCQtCore/MediaPlayer.h>

#include <QDir>
#include <QFileDialog>

Player::Player(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Player)
    , m_instance( new VlcInstance(VlcCommon::args(), this) )
    , m_media(0)
    , m_player( new VlcMediaPlayer(m_instance) )
{
    ui->setupUi(this);

    m_player->setVideoWidget(ui->vlcWidgetVideoPlayer);

    connect( ui->pushButtonLoad, SIGNAL(clicked(bool)),
             this, SLOT(clickedPushButtonLoad()) );
}

Player::~Player()
{
    delete ui;
}

void Player::clickedPushButtonLoad()
{
    QString file( QFileDialog::getOpenFileName(this,
                                               tr("Open File"),
                                               QDir::homePath(),
                                               tr("Multimedia files(*)")));
    if ( file.isEmpty() )
    { return; }

    m_media = new VlcMedia(file, true,  m_instance );
    m_player->open(m_media);
}
