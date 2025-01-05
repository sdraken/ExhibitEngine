#pragma once

#include "Types.hpp"
#include "../loggingSystem/Logger.hpp"
#include "../eventSystem/EventDispatcher.hpp"

#if defined(_WIN32)
    #include "../windowSystem/win32/Win32Window.hpp"
    using WindowManager = Win32Window;
#else
    #error "Unsupported platform"
#endif