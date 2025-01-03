#include "core/Includes.hpp"

void onResize(Event* e) {
    EventResize* eventResize = static_cast<EventResize*>(e);
    std::cout << "new width: " << eventResize->newWidth << std::endl;
    std::cout << "new height: " << eventResize->newHeight << std::endl;
}

int main(int argc, char const *argv[])
{
    EventDispatcher eventDis;
    eventDis.subscribe<EventResize>(onResize);

    WindowManager window(eventDis, 800, 500);

    while(window.handleEvents()){
        eventDis.process();
    }
    
    return 0;
}
