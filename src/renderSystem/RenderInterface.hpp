#pragma once

#include "../eventSystem/EventDispatcher.hpp"
#include "../windowSystem/WindowInterface.hpp"

namespace ExhibitEngine{

    class RenderInterface {
    public:
        RenderInterface(EventDispatcher& eventDispatcher,  Window& window):eventDispatcher(eventDispatcher), window(window){}

        virtual ~RenderInterface() = default;

        virtual void draw() = 0;


    protected:
        EventDispatcher& eventDispatcher;
        Window& window;
    };

}

// can make it so renderer to be used depends on preprocessor defines, right now only vulkan
#include "vulkan/Renderer.hpp"
