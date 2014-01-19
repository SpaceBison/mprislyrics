#include <tcl.h>
#include <lyricsplugin/tclplugin.hpp>

#include <iostream>

const std::string TclPlugin::scriptPath = "./";
const std::string TclPlugin::descriptionCommand = "get_description";
const std::string TclPlugin::lyricsCommand = "get_lyrics";
const std::string TclPlugin::artistKey = "artist";
const std::string TclPlugin::titleKey = "title";
const std::string TclPlugin::albumKey = "album";
const std::string TclPlugin::trackKey = "track";

void TclPlugin::initTcl(const char* argv0)
{
    Tcl_FindExecutable(argv0);
}

TclPlugin::TclPlugin(const std::string& name) : filename(name)
{
    interp = Tcl_CreateInterp();
    Tcl_Init(interp);
    if (Tcl_EvalFile(interp, (scriptPath + "tcl/common.tcl").c_str()) != TCL_OK)
        std::cout << Tcl_GetStringResult(interp) << std::endl;
    if (Tcl_EvalFile(interp, (scriptPath + filename).c_str()) != TCL_OK)
        std::cout << Tcl_GetStringResult(interp) << std::endl;
}

TclPlugin::~TclPlugin()
{
    Tcl_DeleteInterp(interp);
}

std::string TclPlugin::getDescription()
{
    Tcl_Eval(interp, descriptionCommand.c_str());
    return Tcl_GetStringResult(interp);
}

std::string TclPlugin::getLyrics(const TrackInfo& trackinfo)
{
    Tcl_Obj* dict = createTrackInfoDict(trackinfo);
    Tcl_Eval(interp, (lyricsCommand + " {" + Tcl_GetString(dict) + "}").c_str());
    Tcl_DecrRefCount(dict);

    return Tcl_GetStringResult(interp);
}

Tcl_Obj* TclPlugin::createTrackInfoDict(const TrackInfo& trackinfo)
{
    Tcl_Obj* dict = Tcl_NewDictObj();

    Tcl_Obj* artist_k = Tcl_NewStringObj(artistKey.c_str(), artistKey.length());
    Tcl_Obj* artist_obj = Tcl_NewStringObj(trackinfo.artist.c_str(), trackinfo.artist.length());
    Tcl_Obj* title_k = Tcl_NewStringObj(titleKey.c_str(), titleKey.length());
    Tcl_Obj* title_obj = Tcl_NewStringObj(trackinfo.title.c_str(), trackinfo.title.length());
    Tcl_Obj* album_k = Tcl_NewStringObj(albumKey.c_str(), albumKey.length());
    Tcl_Obj* album_obj = Tcl_NewStringObj(trackinfo.album.c_str(), trackinfo.album.length());
    Tcl_Obj* track_k = Tcl_NewStringObj(trackKey.c_str(), trackKey.length());
    Tcl_Obj* track_obj = Tcl_NewIntObj(trackinfo.track);

    Tcl_DictObjPut(interp, dict, artist_k, artist_obj);
    Tcl_DictObjPut(interp, dict, title_k, title_obj);
    Tcl_DictObjPut(interp, dict, album_k, album_obj);
    Tcl_DictObjPut(interp, dict, track_k, track_obj);

    return dict;
}
