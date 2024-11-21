//	Description: The renderer class manages and combines Vulkan components into a functional renderer.
//               Implementation of larger tasks such as managing the pipeline, loading meshes, etc, will
//               be handled within their own classes/structs, but ultimately the renderer class will be
//               what uses these components. 
// 
//               This implementation uses the volk library for loading Vulkan functions.
//
//	Author: Svante Drakenberg

#pragma once
#define VK_USE_PLATFORM_WIN32_KHR

#include <volk/volk.h>
#include "../MemoryAllocation/BumpAllocator.h"

#include <vector>



namespace ExhibitEngine {
    const char* extensions[];


    class Renderer {
    public:
        void initilize();

        void shutDown();

    private:
        VkInstance instance;
        void createInstance();
    };

}
