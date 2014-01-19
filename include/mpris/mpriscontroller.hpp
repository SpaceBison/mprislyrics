#ifndef MPRISCONTROLLER_H
#define MPRISCONTROLLER_H

extern "C"
{
    #include <dbus/dbus.h>
}

#include <common/trackinfo.hpp>

#define MSG_DELAY_MS 50

class MprisController
{
    public:
        MprisController();
        ~MprisController();

        void waitForTrackChange();
        TrackInfo getTrackInfo();

    private:
        DBusConnection *conn;
        TrackInfo trackinfo;

        bool handleMessage(DBusMessage* msg);
        bool handleMprisMessageIter(DBusMessageIter& iter);
        bool handleMpris2MessageIter(DBusMessageIter& iter);
};

#endif /* MPRISCONTROLLER_H */
