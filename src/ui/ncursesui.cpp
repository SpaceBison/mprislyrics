#include <ncurses.h>
#include <csignal>
#include <ui/ncursesui.hpp>

NcursesUI::NcursesUI() : running(true)
{
    thread = std::thread(&NcursesUI::mainLoop, this);
}

NcursesUI::~NcursesUI() {}

void NcursesUI::refresh(std::string lyrics)
{
    clear();
    printw(lyrics.c_str());
    ::refresh();
}

void NcursesUI::mainLoop()
{
    init();
    while(running)
    {
        handleKey();
    }
    exit();
}

void NcursesUI::init()
{
    initscr();
    noecho();
}

void NcursesUI::handleKey()
{
    switch(getch())
    {
    case 'q':
        running = false;
    break;

    /*case KEY_UP:
        scrl(-1);
        ::refresh();
    break;

    case KEY_DOWN:
        scrl(1);
        ::refresh();
    break;*/
    }
}

void NcursesUI::exit()
{
    endwin();
    thread.detach();
    raise(SIGINT);
}
