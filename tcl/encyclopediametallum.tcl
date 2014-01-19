# Encyclopedia Metallum lyrics plugin script v0.1
# TODO: cleaning the garbage chars

package require http

proc get_description {} {
    return "Encyclopedia Metallum lyrics TCL Plugin";
}

proc get_lyrics {trackinfo} {
    global timeout
    # getting the search results
    set artist [dict get $trackinfo artist]
    set title [dict get $trackinfo title]
    set album [dict get $trackinfo album]

    set artist [alphanumeric $artist]
    set title [alphanumeric $title]
    set album [alphanumeric $album]

    set url "http://www.metal-archives.com/search/ajax-advanced/searching/songs/"
	set query [::http::formatQuery songTitle $title bandName $artist releaseTitle $album]

	if { [catch {set token [::http::geturl "$url?$query" -timeout $timeout]} err] } {
        puts "TCL Error: $err (query: $query)"
        return {}
    }
    
	set data [::http::data $token]
    ::http::cleanup $token

    # getting the lyrics
    if { [regexp {lyricsLink_(\d+)} $data -> lyrics_id] == 0 } {
        return {}
    }

    set url "http://www.metal-archives.com/release/ajax-view-lyrics/id/$lyrics_id"
    if { [catch {set token [::http::geturl $url -timeout $timeout]} err] } {
        puts "TCL Error: $err (lyrics_id: $lyrics_id)"
        return {}
    }

    set lyrics [::http::data $token]
    ::http::cleanup $token

    if { [regexp {<em>\(lyrics not available\)</em>} $lyrics] } {
        return {}
    }

    set lyrics [string map {{<br />} {}} $lyrics]
    while {[regsub {\A[\n\s]} $lyrics {} lyrics]} {}
    
    return $lyrics
}
