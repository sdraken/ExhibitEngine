#include "Application.hpp"


//example callback
void onResize(ExhibitEngine::Event* e) {
    ExhibitEngine::EventResize* eventResize = static_cast<ExhibitEngine::EventResize*>(e);
    std::cout << "new width: " << eventResize->newWidth << std::endl;
    std::cout << "new height: " << eventResize->newHeight << std::endl;
}

namespace ExhibitEngine{

    Application::Application():windowManager(eventDispatcher, 800, 500)
    {
        eventDispatcher.subscribe<EventResize>(onResize);
    }

    Application::~Application()
    {
    }

    void Application::run()
    {
        while(windowManager.processEvents()){
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
