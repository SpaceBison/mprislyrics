#include <string>
#include <iostream>
#include <common/trackinfo.hpp>

std::ostream& operator<<(std::ostream& os, TrackInfo& ti)
{
    os << "Artist: " << ti.artist << std::endl;
    os << "Title: " << ti.title << std::endl;
    os << "Album: " << ti.album << std::endl;
    os << "Track: " << ti.track << std::endl;
}

bool operator==(TrackInfo& one, TrackInfo& two)
{
    return one.artist == two.artist &&
           one.title == two.title &&
           one.album == two.album &&
           one.track == two.track;
}
