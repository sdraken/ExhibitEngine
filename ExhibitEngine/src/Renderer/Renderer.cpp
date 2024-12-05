//	Description: Implementation of Renderer class
//
//	Author: Svante Drakenberg

//Just for intelisense



#define VOLK_IMPLEMENTATION
#include "Renderer.h"



namespace ExhibitEngine {
    
    void Renderer::initilize(HINSTANCE instanceHandle, HWND windowHandle)
    {
        //as Renderer gets more high abstraction it will call more than just context 
        context.initialize(instanceHandle, windowHandle);

    }

    void Renderer::shutDown() const
    {
        //as Renderer gets more high abstraction it will call more than just context 
        context.shutDown();
    }

}




