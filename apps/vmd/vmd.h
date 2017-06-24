#ifndef VMD_H
#define VMD_H

#include <QWidget>
#include <QSettings>

#include "datapackage.h"

namespace Ui { class VMD; }
namespace gtqt { class PeerLink; }
class QHostAddress;

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
    void clickPushButtonConnect();
    void clickPushButtonAdd();
    void clickPushButtonRemove();
    void clickPushButtonLoad();

    void receive( gtqt::DataPackage<gtqt::ClientType1> const& );

    void timeout();

private:
    void listen(QHostAddress const&, quint16 port);

private:
    Ui::VMD *ui;
    QSettings m_settings;

    QString const m_addrregexp;
    gtqt::PeerLink* m_link;
};

#endif // VMD_H
