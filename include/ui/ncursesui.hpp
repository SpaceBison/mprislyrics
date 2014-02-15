#ifndef NCURSESUI_HPP
#define NCURSESUI_HPP

#include <string>
#include <thread>
#include <ncurses.h>
#include <common/trackinfo.hpp>

class NcursesUI
{
    public:
        NcursesUI();
        ~NcursesUI();
        void setText(std::string text);
        void setMsg(std::string msg);
    
    private:
        void mainLoop();
        void init();
        void handleKey();
        void exit();

        bool running;
        std::thread thread;
        WINDOW* textPad;
        int scroll;
        int rows, cols;
};

#endif /* NCURSESUI_HPP */ 
