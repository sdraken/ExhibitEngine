#pragma once

#include "eventSystem/EventDispatcher.hpp"
#include "windowSystem/WindowInterface.hpp"
#include "renderSystem/RenderInterface.hpp"

namespace ExhibitEngine{

    class Application
    {
    public:
        Application(/* args */);
        ~Application();
        void run();
    private:
        EventDispatcher eventDispatcher;
        Window window;
        Renderer renderer;
    };

}