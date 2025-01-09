#include "Application.hpp"


//example callback
void onResize(ExhibitEngine::Event* e) {
    ExhibitEngine::EventResize* eventResize = static_cast<ExhibitEngine::EventResize*>(e);
    std::cout << "new width: " << eventResize->newWidth << std::endl;
    std::cout << "new height: " << eventResize->newHeight << std::endl;
}

namespace ExhibitEngine{

    Application::Application():window(eventDispatcher, 800, 500), renderer(eventDispatcher, window)
    {
        eventDispatcher.subscribe<EventResize>(onResize); //remove later
    }

    Application::~Application()
    {
    }

    void Application::run()
    {
        while(window.processEvents()){
            eventDispatcher.process();
        }
    
    }

}


//later on we seperate game from game engine.
int main(int argc, char const *argv[])
{
    ExhibitEngine::Application application;

    application.run();
    
    return 0;
}
