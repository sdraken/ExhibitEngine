#pragma once
#include "../core/Types.hpp"
#include "../eventSystem/EventDispatcher.hpp"

namespace ExhibitEngine{

class WindowInterface {
    
    public:
        WindowInterface(EventDispatcher& eventDispatcher):
            eventDispatcher(eventDispatcher),
            running(true),
            beenResized(false){
        }


        virtual ~WindowInterface() = default;

        virtual void createWindow(int width, int height) = 0;
        virtual bool processEvents() = 0;
        virtual void closeWindow() = 0;
    protected:
        const char* title = "ExhibitEngine";
        EventDispatcher& eventDispatcher;
        bool running;
        bool beenResized;
    };

}

#if defined(_WIN32)
    #include "win32/Window.hpp"
#else
    #error "Unsupported platform"
#endif