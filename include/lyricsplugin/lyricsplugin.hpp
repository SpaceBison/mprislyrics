#ifndef LYRICSPLUGIN_H
#define LYRICSPLUGIN_H

#include <string>
#include <common/trackinfo.hpp>

class LyricsPlugin
{
    public:
        virtual ~LyricsPlugin() {}
        virtual std::string getDescription() { return "Dummy Plugin"; }
        virtual std::string getLyrics(const TrackInfo& trackinfo) = 0;

    private:
        /* add your private declarations */
};

#endif /* LYRICSPLUGIN_H */
