#include <ncurses.h>
#include <csignal>
#include <ui/ncursesui.hpp>

NcursesUI::NcursesUI() : running(true)
{
    thread = std::thread(&NcursesUI::mainLoop, this);
}

NcursesUI::~NcursesUI()
{
    exit();
}

void NcursesUI::setText(std::string text)
{
    wclear(textPad);
    waddstr(textPad, text.c_str());
    scroll = 0;
    getmaxyx(stdscr, rows, cols);
    prefresh(textPad, 0, 0,
                0, 0,
                rows-2, cols-1);
}

void NcursesUI::setMsg(std::string msg)
{
    getmaxyx(stdscr, rows, cols);
    attron(COLOR_PAIR(1));
    mvaddstr(rows-1, 0, std::string(cols, ' ').c_str());
    mvaddstr(rows-1, 0, msg.c_str());
    attroff(COLOR_PAIR(1));
    refresh();
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
    getmaxyx(stdscr, rows, cols);
    textPad = newpad(1024, 256);
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    refresh();
}

void NcursesUI::handleKey()
{
    switch(getch())
    {
    case 'q':
        running = false;
    break;

    case KEY_UP:
    case 'w':
        getmaxyx(stdscr, rows, cols);
        prefresh(textPad, scroll ? --scroll : scroll, 0,
                0, 0,
                rows-2, cols-1);
    break;

    case KEY_DOWN:
    case 's':
        getmaxyx(stdscr, rows, cols);
        prefresh(textPad, ++scroll, 0,
                0, 0,
                rows-2, cols-1);
    break;
    }
}

void NcursesUI::exit()
{
    endwin();
    thread.detach();
    raise(SIGINT);
}
