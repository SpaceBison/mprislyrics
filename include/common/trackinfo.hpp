#ifndef TRACKINFO_H
#define TRACKINFO_H

#include <string>
#include <iostream>

struct TrackInfo
{
    std::string artist;
    std::string title;
    std::string album;
    int track;
};

std::ostream& operator<<(std::ostream& os, TrackInfo& ti);
bool operator==(TrackInfo& one, TrackInfo& two);

#endif /* TRACKINFO_H */ 
