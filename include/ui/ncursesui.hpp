#ifndef NCURSESUI_HPP
#define NCURSESUI_HPP

#include <string>
#include <thread>
#include <common/trackinfo.hpp>

class NcursesUI
{
    public:
        NcursesUI();
        virtual ~NcursesUI();
        void refresh(std::string lyrics);
    
    private:
        void mainLoop();
        void init();
        void handleKey();
        void exit();

        bool running;
        std::thread thread;
};

#endif /* NCURSESUI_HPP */ 
