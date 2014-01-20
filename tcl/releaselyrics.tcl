# releaselyrics plugin script v0.1

package require http

proc get_description {} {
    return "ReleaseLyrics TCL Plugin";
}

proc get_lyrics {trackinfo} {
    global timeout
    # getting the search results
    set artist [dict get $trackinfo artist]
    set title [dict get $trackinfo title]

    set url "http://www.releaselyrics.com/search.php"
    set query [::http::formatQuery q "$artist $title"]

    if { [catch {set token [::http::geturl "$url?$query" -timeout $timeout]} err] } {
        puts "TCL Error: $err (query: $query)"
        return {}
    }

    set data [::http::data $token]
    ::http::cleanup $token

    # getting the lyrics

    if { [regexp {\|(.+?)$} $data -> link] == 0 } {
        return {}
    }

    set url "http://www.releaselyrics.com/$link"
    if { [catch {set token [::http::geturl $url -timeout $timeout]} err] } {
        puts "TCL Error: $err (URL: $url)"
        return {}
    }

    set data [::http::data $token]
    ::http::cleanup $token

    if { [regexp {<div id="id-content" class="content">(.+?)</div>} $data -> lyrics] == 0} {
        return {}
    }

    set lyrics [string map {"\n" {} {<br>} "\n"} $lyrics]
    #set lyrics [string map {{<br />} {}} $lyrics]
    #while {[regsub {\A[\n\s]} $lyrics {} lyrics]} {}

    return $lyrics
}
