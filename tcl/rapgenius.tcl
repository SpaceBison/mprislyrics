# rapgenius plugin script v0.1

package require http

proc get_description {} {
    return "Rapgenius TCL Plugin";
}

proc get_lyrics {trackinfo} {
    # getting the search results
    set artist [dict get $trackinfo artist]
    set title [dict get $trackinfo title]

    # get rid of problematic characters
    set artist [regsub -all {\W} $artist { }]
    set title [regsub -all {\W} $title { }]
    
    set url "http://rapgenius.com/search"
	set query [::http::formatQuery hide_unexplained_songs false q "$artist $title"]
    set query [string map {"%20" +} $query]

	if { [catch {set token [::http::geturl "$url?$query" -timeout 20000]} err] } { # we're supposed to use GET
        puts "TCL Error: $err (query: $query)"
        return {}
    }
    
	set data [::http::data $token]
    ::http::cleanup $token
    
    # getting the lyrics
    
    if { [regexp {<li class=\"search_result\">.*?<a href=\"(.+?)\" class=\" song_link\">} $data -> link] == 0 } {
        return {}
    }

    set url "http://rapgenius.com$link"
    if { [catch {set token [::http::geturl $url -timeout 20000]} err] } {
        puts "TCL Error: $err (URL: $url)"
        return {}
    }
    
    set data [::http::data $token]
    ::http::cleanup $token

    if { [regexp {<div class=\"lyrics\".*?<p>(.+?)</p>} $data -> lyrics] == 0} {
        return {}
    }

    set lyrics [regsub -all {<a.*?>(.+?)</a>} $lyrics {\1}]
    set lyrics [string map {{<br>} {}} $lyrics]
    
    return $lyrics
}
