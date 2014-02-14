#include <iostream>
#include <vector>
#include <csignal>
#include <dbus-c++/dbus.h>
#include <common/trackinfo.hpp>
#include <lyricsplugin/lyricsplugin.hpp>
#include <lyricsplugin/tclplugin.hpp>
#include <dbus/dbusproperties.hpp>
#include <dbus/mpris2player.hpp>

DBus::BusDispatcher dispatcher;
std::vector<LyricsPlugin*> lyrics_plugins;
MPRIS2Player* player;
TrackInfo old_ti;


void display_lyrics()
{
    TrackInfo ti = player->getTrackInfo();

    if(ti == old_ti)
        return;

    old_ti = ti;

    std::cout << std::string( 70, '\n' ) << ti.artist << " - " << ti.title << std::endl;

    bool found = false;
    for(LyricsPlugin* lp : lyrics_plugins)
    {
        std::cout << lp->getDescription() << "..." << std::endl;
        std::string lyrics = lp->getLyrics(ti);
        if(lyrics.size())
        {
            std::cout << std::endl << lyrics << std::endl;
            found = true;
            break;
        }
    }
    if(!found)
        std::cout << "No results :(" << std::endl;
}

void signal_handler(int signal)
{
    switch(signal)
    {
    case SIGINT:
    case SIGTERM:
        delete player;
        lyrics_plugins.clear();
        dispatcher.leave();
        exit(0);
    break;
    }
}

int main(int argc, char **argv)
{
    TclPlugin::initTcl(argv[0]);

    lyrics_plugins.push_back(new TclPlugin("tcl/darklyrics.tcl"));
    lyrics_plugins.push_back(new TclPlugin("tcl/encyclopediametallum.tcl"));
    lyrics_plugins.push_back(new TclPlugin("tcl/lyricstime.tcl"));
    lyrics_plugins.push_back(new TclPlugin("tcl/releaselyrics.tcl"));
    lyrics_plugins.push_back(new TclPlugin("tcl/tekstowo.tcl"));
    //lyrics_plugins.push_back(new TclPlugin("tcl/rapgenius.tcl"));
    lyrics_plugins.push_back(new TclPlugin("tcl/lyricwiki.tcl"));

    DBus::default_dispatcher = &dispatcher;
    DBus::Connection conn = DBus::Connection::SessionBus();
    DBus::CallMessage request("org.freedesktop.DBus", "/", "org.freedesktop.DBus", "ListNames");
    DBus::Message reply = conn.send_blocking(request);
    DBus::MessageIter ri = reply.reader();
    std::vector<std::string> names;
    ri >> names;

    const std::string mpris2path("/org/mpris/MediaPlayer2");
    const std::string mpris2interfacePrefix("org.mpris.MediaPlayer2");
    std::string playerBusName;

    for(std::string s : names)
    {
        if(!s.compare(0, mpris2interfacePrefix.length(), mpris2interfacePrefix))
        {
            playerBusName = s;
            break;
        }
    }

    std::cout << "*** Connecting to " << playerBusName << std::endl;

    player = new MPRIS2Player(conn, mpris2path.c_str(), playerBusName.c_str());
    DBusProperties dbus(conn);

    dbus.setCallback(display_lyrics);

    dispatcher.enter();

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    return 0;
}

