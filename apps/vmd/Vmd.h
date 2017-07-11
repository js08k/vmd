#ifndef VMD_H
#define VMD_H

#include <QObject>

namespace gui { class VMD; }
namespace gui { class VideoWidget; }
namespace dvd { class StreamPlayer; }
namespace dvd { class MediaContext; }

class gui::VMD
        : public QObject
{
    Q_OBJECT

public:
    explicit VMD( QObject* parent = 0 );
    void setVideoWidget( gui::VideoWidget* widget );

signals:
    void setConnected(bool);

private slots:
    void toggledConnect();
    void loadMedia( QString const& );
    void peerAddressChanged( QString const& );
    void hostAddressChanged( QString const& );

private:
    gui::VideoWidget* m_widget;
    QString const m_addrexp;
    dvd::StreamPlayer* m_player;
    dvd::MediaContext* m_context;
};

#endif // VMD_H
