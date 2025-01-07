#pragma once

#include "core/Types.hpp"
#include "loggingSystem/Logger.hpp"
#include "eventSystem/EventDispatcher.hpp"

#if defined(_WIN32)
    #include "windowSystem/win32/Win32Window.hpp"
    using WindowManager = ExhibitEngine::Win32Window;
#else
    #error "Unsupported platform"
#endif

namespace ExhibitEngine{

    class Application
    {
    public:
        Application(/* args */);
        ~Application();
        void run();
    private:
        EventDispatcher eventDispatcher;
        WindowManager windowManager;
    };

}