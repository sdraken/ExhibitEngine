#include "Renderer.hpp"



namespace ExhibitEngine{

    Renderer::Renderer(EventDispatcher& eventDispatcher, Window& window):RenderInterface(eventDispatcher, window)
    {
        init_vulkan();
    }

    Renderer::~Renderer()
    {
    }

    void Renderer::init_vulkan()
    {   


    }

    void Renderer::draw()
    {   


    }

}