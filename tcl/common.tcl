#### global variables ####
# http timeout
set timeout 20000

#### common procedures ####

# strip the string from non-alphanumeric chars
proc alphanumeric { string } {
    set string [string map {' {}} $string]
    set string [regsub -all {\W} $string { }]
    return $string
}

# taken from Artur Krzewik's post on http://wiki.tcl.tk/3841
proc stringFuzzyMatch {str1 str2} {
    set l1 [lsort -unique [trigrams2 $str1]]
    set l2 [lsort -unique [trigrams2 $str2]]
    set g 0
    set i1 0
    set i2 0
    set len1 [llength $l1]
    set len2 [llength $l2]
    set t1 [lindex $l1 $i1]
    set t2 [lindex $l2 $i2]
    set l 0
    while {$i1<$len1 && $i2<$len2} {
        incr l
        set r [string compare $t1 $t2]
        # better than switch statement
        if {$r==0} {
            incr i1
            set t1 [lindex $l1 $i1]
            incr i2
            set t2 [lindex $l2 $i2]
            incr g
        } elseif {$r==-1} {
            incr i1
            set t1 [lindex $l1 $i1]
        } else {
            incr i2
            set t2 [lindex $l2 $i2]
        }
    }
    expr {100*$g/$l}
  }
  proc trigrams2 string {
    set rez [list]
    set str " $string "
    set l [expr {[string length $str]-2}]
    for {set x 0} {$x<$l} {incr x} {
        lappend rez [string range $str $x [expr {$x+2}]]
    }
    return $rez
  }
