# lyrics.time plugin script v0.1

package require http

proc get_description {} {
    return "Lyricstime TCL Plugin";
}

proc get_lyrics {trackinfo} {
    global timeout
    
    # getting the search results
    set artist [dict get $trackinfo artist]
    set title [dict get $trackinfo title]

    set artist [alphanumeric $artist]
    set title [alphanumeric $title]
    
    set url "http://www.lyricstime.com/search/"
	set query [::http::formatQuery q "$artist $title"]
    set query [string map {"%20" +} $query]

	if { [catch {set token [::http::geturl "$url?$query" -timeout $timeout]} err] } {
        puts "TCL Error: $err (query: $query)"
        return {}
    }
    
	set data [::http::data $token]
    ::http::cleanup $token
    
    # getting the lyrics
    
    if { [regexp {<li><a href=\"(.+?)\">} $data -> link] == 0 } {
        return {}
    }

    set url "http://www.lyricstime.com$link"
    if { [catch {set token [::http::geturl $url -timeout $timeout]} err] } {
        puts "TCL Error: $err (URL: $url)"
        return {}
    }
    
    set data [::http::data $token]
    ::http::cleanup $token

    regexp "<div id=\"songlyrics\".+?<p>(.+?)</p>" $data -> lyrics

    set lyrics [string map {{<br />} {}} $lyrics]
    while {[regsub {\A[\n\s]} $lyrics {} lyrics]} {}
    
    return $lyrics
}
