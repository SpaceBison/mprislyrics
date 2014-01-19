#ifndef TCLPLUGIN_H
#define TCLPLUGIN_H

extern "C"
{
    #include <tcl.h>
}

#include <lyricsplugin/lyricsplugin.hpp>

class TclPlugin : public LyricsPlugin
{
    public:
        static const std::string scriptPath;
        static const std::string descriptionCommand;
        static const std::string lyricsCommand;
        static const std::string artistKey;
        static const std::string titleKey;
        static const std::string albumKey;
        static const std::string trackKey;

        static void initTcl(const char* argv0);

        TclPlugin(const std::string& name);
        ~TclPlugin();
        std::string getDescription();
        std::string getLyrics(const TrackInfo& trackinfo);

    private:
        Tcl_Interp* interp;
        std::string filename;

        Tcl_Obj* createTrackInfoDict(const TrackInfo& trackinfo);

};

#endif /* TCLPLUGIN_H */
