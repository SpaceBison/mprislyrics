# lyricswiki plugin script v0.1

package require http

proc get_description {} {
    return "Tekstowo TCL Plugin";
}

proc get_lyrics {trackinfo} {
    global timeout
    # getting the search results
    set artist [dict get $trackinfo artist]
    set title [dict get $trackinfo title]

    set artist_f [::http::formatQuery "" $artist]
    set title_f [::http::formatQuery "" $title]

    regsub {^=} $artist_f "" artist_f
    regsub {^=} $title_f "" title_f
    set artist_f [string map {"%20" +} $artist_f]
    set title_f [string map {"%20" +} $title_f]
    
    set url "http://www.tekstowo.pl/szukaj,wykonawca,$artist_f,tytul,$title_f.html"

	if { [catch {set token [::http::geturl "$url" -timeout $timeout]} err] } { # GET
        puts "TCL Error: $err (url: $url)"
        return {}
    }
    
	set data [::http::data $token]
    ::http::cleanup $token

    if { [regexp {<h2>Znalezione utwory:</h2>.*?<a href="(.*?)" class="title" title="(.*?)">} $data -> link lyrics_title] == 0 } {
        return {}
    }

    # tekstowo tends to fetch some really questionable matches
    if { [stringFuzzyMatch [string tolower $lyrics_title] [string tolower "$artist - $title"]] < 50 } {
        return {}
    }
        

    set url "http://www.tekstowo.pl$link"

    if { [catch {set token [::http::geturl $url -timeout $timeout]} err] } {
        puts "TCL Error: $err (URL: $url)"
        return {}
    }

    set data [::http::data $token]
    ::http::cleanup $token

    if {[regexp "<h2>Tekst piosenki:</h2>(.+?)<p>&nbsp;</p>" $data -> lyrics] == 0 } {
        return ""
    } else {
        regsub -all {\s\s+}  $lyrics "" lyrics
        return [string map {"\n" "" {<br />} "\n"} $lyrics]
    }
}
