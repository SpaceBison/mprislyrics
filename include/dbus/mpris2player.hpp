#ifndef MPRIS2PLAYER_H
#define MPRIS2PLAYER_H

#include <dbus-c++/dbus.h>
#include <dbus-c++/property.h>
#include <dbus/dbusproperties.hpp>
#include <dbus/playerproxy.hpp>
#include <common/trackinfo.hpp>

class MPRIS2Player :
    public org::mpris::MediaPlayer2::Player_proxy,
    public DBus::ObjectProxy
{
    public:
        MPRIS2Player(DBus::Connection& connection,
                     const char* path,
                     const char* name);

        TrackInfo getTrackInfo();

    private:
        int _getInt(::DBus::MessageIter mi);
        std::string _getString(::DBus::MessageIter mi);
};

#endif /* MPRIS2PLAYER_H */ 
