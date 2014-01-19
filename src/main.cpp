#include <iostream>
#include <vector>
#include <common/trackinfo.hpp>
#include <lyricsplugin/lyricsplugin.hpp>
#include <lyricsplugin/tclplugin.hpp>
#include <mpris/mpriscontroller.hpp>

int main(int argc, char **argv)
{
    TclPlugin::initTcl(argv[0]);
    MprisController mpris;
    TrackInfo ti;
    std::vector<LyricsPlugin*> lyrics_plugins;

    lyrics_plugins.push_back(new TclPlugin("tcl/darklyrics.tcl"));
    lyrics_plugins.push_back(new TclPlugin("tcl/encyclopediametallum.tcl"));
    lyrics_plugins.push_back(new TclPlugin("tcl/lyricstime.tcl"));
    lyrics_plugins.push_back(new TclPlugin("tcl/tekstowo.tcl"));
    //lyrics_plugins.push_back(new TclPlugin("tcl/rapgenius.tcl"));
    lyrics_plugins.push_back(new TclPlugin("tcl/lyricwiki.tcl"));

    while(true)
    {
        mpris.waitForTrackChange();
        ti = mpris.getTrackInfo();

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

    return 0;
}

