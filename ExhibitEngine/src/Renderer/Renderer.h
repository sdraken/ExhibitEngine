//	Description: The renderer class manages and combines Vulkan components into a functional renderer.
//               Implementation of larger tasks such as managing the pipeline, loading meshes, etc, will
//               be handled within their own classes/structs, but ultimately the renderer class will be
//               what uses these components. 
// 
//               This implementation uses the volk library for loading Vulkan functions.
//
//	Author: Svante Drakenberg

#pragma once
#include "volk.h"
#include "vk_mem_alloc.h"
#include "glm/vec3.hpp"
#include "stb_image.h"
#include "tiny_obj_loader.h"

#include "../MemoryAllocation/BumpAllocator.h"
#include "../Logging/Logger.h"
#include "../UI/Window.h"
#include "Context.h"
#include "SwapChainManager.h"
#include "PipelineManager.h"
#include "CommandBufferManager.h"

#include <cstring>


namespace ExhibitEngine {

    class Renderer {
    public:
        Renderer(const Window& window);
        ~Renderer();

        void drawFrame();
    private:
        Context vulkanContext;
        SwapChainManager swapChainManager;
        PipelineManager pipelineManager;
        CommandBufferManager commandBufferManager;

        VkDevice device;
        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;
        uint32_t currentFrame = 0;
        bool& framebufferResized;

        void createSyncObjects();
    };

}
