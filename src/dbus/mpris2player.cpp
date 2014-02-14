#include <cstdlib>
#include <cstdio>
#include <dbus-c++/property.h>
#include <dbus/mpris2player.hpp>

MPRIS2Player::MPRIS2Player(DBus::Connection& connection, const char* path, const char* name) :
    DBus::ObjectProxy(connection, path, name) {}

TrackInfo MPRIS2Player::getTrackInfo()
{
    TrackInfo trackinfo;
    std::map< std::string, ::DBus::Variant > metadata = Metadata();

    if(metadata.find("xesam:artist") != metadata.end())
        trackinfo.artist = _getString(metadata["xesam:artist"].reader());
    else
        trackinfo.artist = "";

    if(metadata.find("xesam:title") != metadata.end())
        trackinfo.title = _getString(metadata["xesam:title"].reader());
    else
        trackinfo.title = "";

    if(metadata.find("xesam:album") != metadata.end())
        trackinfo.album = _getString(metadata["xesam:album"].reader());
    else
        trackinfo.album = "";

    if(metadata.find("xesam:trackNumber") != metadata.end())
        trackinfo.track = _getInt(metadata["xesam:trackNumber"].reader());
    else
        trackinfo.track = 0;

    return trackinfo;
}

int MPRIS2Player::_getInt(::DBus::MessageIter mi)
{
    if(mi.type() == 'v')
    {
        ::DBus::MessageIter rec = mi.recurse();
        return _getInt(rec);
    }
    if(mi.type() == 'y')
        return mi.get_byte();
    if(mi.type() == 'b')
        return mi.get_bool();
    if(mi.type() == 'n')
        return mi.get_int16();
    if(mi.type() == 'q')
        return mi.get_uint16();
    if(mi.type() == 'i')
        return mi.get_int32();
    if(mi.type() == 'u')
        return mi.get_uint32();
    if(mi.type() == 'x')
        return mi.get_int64();
    if(mi.type() == 't')
        return mi.get_uint64();
    if(mi.type() == 'd')
        return mi.get_double();
    if(mi.type() == 's')
        return atoi(mi.get_string());
    if(mi.type() == 'a');
    {
        ::DBus::MessageIter rec = mi.recurse();
        return _getInt(rec);
    }
    return 0;
}                    
                     
std::string MPRIS2Player::_getString(::DBus::MessageIter mi)
{
    char str[16] = "";
    if(mi.type() == 'v')
    {
        ::DBus::MessageIter rec = mi.recurse();
        return _getString(rec);
    }
    if(mi.type() == 'y')
    {
        sprintf(str, "%d", mi.get_byte());
        return str;
    }
    if(mi.type() == 'b')
    {
        sprintf(str, "%d", mi.get_bool());
        return str;
    }
    if(mi.type() == 'n')
    {
        sprintf(str, "%d", mi.get_int16());
        return str;
    }
    if(mi.type() == 'q')
    {
        sprintf(str, "%u", mi.get_uint16());
        return str;
    }
    if(mi.type() == 'i')
    {
        sprintf(str, "%d", mi.get_int32());
        return str;
    }
    if(mi.type() == 'u')
    {
        sprintf(str, "%u", mi.get_uint32());
        return str;
    }
    if(mi.type() == 'x')
    {
        sprintf(str, "%lld", mi.get_int64());
        return str;
    }
    if(mi.type() == 't')
    {
        sprintf(str, "%llu", mi.get_uint64());
        return str;
    }
    if(mi.type() == 'd')
    {
        sprintf(str, "%f", mi.get_double());
        return str;
    }
    if(mi.type() == 's')
        return mi.get_string();
    if(mi.type() == 'a')
    {
        ::DBus::MessageIter rec = mi.recurse();
        return _getString(rec);
    }
    return 0;
}
