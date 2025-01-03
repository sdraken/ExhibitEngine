#pragma once
#include "../eventSystem/EventDispatcher.hpp"

class WindowInterface {
public:
    WindowInterface(EventDispatcher& eventDispatcher, int width, int height):
        eventDispatcher(eventDispatcher),
        width(width),
        height(height){
    }


    virtual ~WindowInterface() = default;

    virtual void createWindow() = 0;
    virtual bool handleEvents() = 0;
    virtual void closeWindow() = 0;
protected:
    int width;
    int height;
    const char* title = "ExhibitEngine";
    EventDispatcher& eventDispatcher;
    inline static EventResize eventResize;
};