/* UML Class Diagram
@startuml

class MediaFrame <<gtqt.MediaFrame>> {
{static}frames(QByteArray, Action) : vector<MediaFrame>
+MediaFrame()
+MediaFrame(gtqt::MediaFrame)
+encrypt(QString)
+decrypt(QString)
}

@enduml
*/
#include "dvd/MediaFrame.h"

#include <QString>

dvd::MediaFrame::MediaFrame()
{
}

dvd::MediaFrame::MediaFrame( gtqt::MediaFrame const& other )
    : gtqt::MediaFrame(other)
{
}

QList<dvd::MediaFrame> dvd::MediaFrame::frames(
        QByteArray data, Action action )
{
    int const maxsize(gtqt::MediaFrame::MaxFrameSize);
    QList<dvd::MediaFrame> frms;
    int index(0);

    do
    {
        QByteArray frame_data( data.mid(index, qMin(data.size()-index, maxsize)) );
        index += frame_data.size();

        dvd::MediaFrame frm;
        frm.set_action( proto::MediaFrame::FrameAction(action) );
        frm.set_data( frame_data.data(), frame_data.size() );
        frms << frm;

        action = Append;
    } while ( index < data.size() );

    return frms;
}

void dvd::MediaFrame::encrypt( QString const& key )
{
    // No encryption actually happening yet...
    Q_UNUSED(key);

    // Modify the flags to indicate the state of the data
    switch ( action() )
    {
    case gtqt::MediaFrame::Append:
    case gtqt::MediaFrame::Flush:
        break;
    case gtqt::MediaFrame::DecryptAppend:
        set_action( gtqt::MediaFrame::Append );
        break;
    case gtqt::MediaFrame::DecryptFlush:
        set_action( gtqt::MediaFrame::Flush );
        break;
    }

}

void dvd::MediaFrame::decrypt( QString const& key )
{
    // No decryption actually happening yet...
    Q_UNUSED(key);

    // Modify the flags to indicate the state of the data
    switch ( action() )
    {
    case gtqt::MediaFrame::Append:
        set_action( gtqt::MediaFrame::DecryptAppend );
        break;
    case gtqt::MediaFrame::Flush:
        set_action( gtqt::MediaFrame::DecryptFlush );
        break;
    case gtqt::MediaFrame::DecryptAppend:
    case gtqt::MediaFrame::DecryptFlush:
        break;
    }
}
