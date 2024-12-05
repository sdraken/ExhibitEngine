//	Description: The renderer class manages and combines Vulkan components into a functional renderer.
//               Implementation of larger tasks such as managing the pipeline, loading meshes, etc, will
//               be handled within their own classes/structs, but ultimately the renderer class will be
//               what uses these components. 
// 
//               This implementation uses the volk library for loading Vulkan functions.
//
//	Author: Svante Drakenberg

#pragma once
#ifndef VK_USE_PLATFORM_WIN32_KHR
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#include "volk.h"
#include "VulkanContext.h"

#include "../MemoryAllocation/BumpAllocator.h"
#include "../Logging/Logger.h"

#include <cstring>


namespace ExhibitEngine {

    class Renderer {
    public:
        void initilize(HINSTANCE instanceHandle, HWND windowHandle);

        void shutDown() const;

    private:
        VulkanContext context;
        

    };

}
