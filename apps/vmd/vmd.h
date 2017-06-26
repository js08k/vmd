#ifndef VMD_H
#define VMD_H

#include <QWidget>
#include <QSettings>

#include "datapackage.h"
#include "dvd/dvdcontext.h"
#include "dvd/streambuffer.h"

namespace Ui { class VMD; }
namespace gtqt { class PeerLink; }
class QHostAddress;
class QThread;
class QMediaPlayer;

namespace dvd { class StreamPlayer; }

class VMD
        : public QWidget
{
    Q_OBJECT

public:
    explicit VMD(QWidget *parent = 0);
    ~VMD();

private slots:
    void lineEditUserNameFinished();
    void lineEditHostAddressFinished();
    void lineEditSimulatedDelayFinished();
    void lineEditPeerAddressFinished();
    void dvdStartTitleChanged(int);
    void dvdStartChapterChanged(int);

    void clickPushButtonConnect();
    void clickPushButtonAdd();
    void clickPushButtonRemove();
    void clickPushButtonLoad();
    void clickPushButtonPlayPause();

    void setTitle( QString const& title );

    void receive( gtqt::DataPackage<gtqt::ClientType1> const& );

    void timeout();

private:
    void listen(QHostAddress const&, quint16 port);

private:
    Ui::VMD *ui;
    QSettings m_settings;

    QString const m_addrregexp;
    gtqt::PeerLink* m_link;

    QThread* m_mediaThread;
    DvDContext* m_mediaContext;
//    QMediaPlayer* m_player;
//    StreamBuffer m_buffer;
    dvd::StreamPlayer* m_player;
};

#endif // VMD_H
