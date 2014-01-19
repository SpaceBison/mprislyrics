# lyricswiki plugin script v0.1

package require http

proc get_description {} {
    return "LyricsWiki TCL Plugin";
}

proc get_lyrics {trackinfo} {
    global timeout
    # getting the search results
    set artist [dict get $trackinfo artist]
    set title [dict get $trackinfo title]
    
    set url "http://lyrics.wikia.com/api.php"
	set query [::http::formatQuery artist $artist song $title fmt realjson]

	if { [catch {set token [::http::geturl "$url?$query" -timeout $timeout]} err] } { # GET
        puts "TCL Error: $err (query: $query)"
        return {}
    }
    
	set data [::http::data $token]
    ::http::cleanup $token

    # getting the lyrics .+?<a href=\"(.+?)\"
    if { [regexp {\"lyrics\":\"(.+?)\"} $data -> lyrics] == 0 } {
        return {}
    } elseif { [regexp {Not found} $lyrics] } {
        return {}
    } else {
        return [string map {{\n} "\n"} $lyrics]
    }
}
