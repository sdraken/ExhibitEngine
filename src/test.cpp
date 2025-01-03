#include "core/Types.hpp"
#include "eventSystem/EventDispatcher.hpp"
#include "windowSystem/Win32Window.hpp"

#include <iostream> //removable later




void onResize(Event* e) {
    EventResize* eventResize = static_cast<EventResize*>(e);
    std::cout << "new width: " << eventResize->newWidth << std::endl;
    std::cout << "new height: " << eventResize->newHeight << std::endl;
}

int main(int argc, char const *argv[])
{
    EventDispatcher eventDis;

    eventDis.subscribe<EventResize>(onResize);


    std::cout << "Hello ExhibitEngine!" << std::endl;

    WindowManager window(eventDis, 800, 500);

    while(window.handleEvents()){
        eventDis.process();
    }
    return 0;
}
