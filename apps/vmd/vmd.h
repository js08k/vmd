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
class LinkManager;

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

    void receive( gtqt::DataPackage<gtqt::MediaInfo> const& );
    void receive( gtqt::DataPackage<gtqt::StreamRequest> const& );

private:
    void listen(QHostAddress const&, quint16 port);

private:
    Ui::VMD *ui;

    LinkManager* m_link;

    // Logical Grouping for containing the MediaInfo
    struct
    {
        bool isNull;
        gtqt::MediaInfo msg;
    } m_mediainfo;

    QSettings m_settings;
    QString const m_addrregexp;

    QThread* m_mediaThread;
    dvd::StreamPlayer* m_player;
    dvd::MediaContext* m_context;
};

#endif // VMD_H
