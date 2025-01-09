#pragma once

#include "../RenderInterface.hpp"
#include <volk.h>
#include <vector>






//predefines for extensions and validation layers depending on debug
#ifdef NDEBUG
    #define LAYERNAMES {}
    #define EXTENSIONNAMES {VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME}
    #define ENABLEVALIDATINLAYERS false
#else
    #define LAYERNAMES {"VK_LAYER_KHRONOS_validation"}
    #define EXTENSIONNAMES { VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME, VK_EXT_DEBUG_UTILS_EXTENSION_NAME}
    #define ENABLEVALIDATINLAYERS true
#endif

#define DEVICEEXTENSIONNAMES {VK_KHR_SWAPCHAIN_EXTENSION_NAME}

namespace ExhibitEngine
{
    class Renderer : public RenderInterface{
    public:

        //initializes everything in the engine
        Renderer(EventDispatcher& eventDispatcher, Window& window);

        //shuts down the engine
        ~Renderer();

        //draw loop
        void draw();



    private:
        std::vector<const char*> extensions = EXTENSIONNAMES;
        std::vector<const char*> layers = LAYERNAMES;
        std::vector<const char*> deviceExtensions = DEVICEEXTENSIONNAMES;

        int frameNumber {0};
        VkInstance instance;// Vulkan library handle
        VkDebugUtilsMessengerEXT debug_messenger;// Vulkan debug output handle
        VkPhysicalDevice chosenGPU;// GPU chosen as the default device
        VkDevice device; // Vulkan device for commands
        VkSurfaceKHR surface;// Vulkan window surface


        void init_vulkan();
        void init_swapchain();
        void init_commands();
        void init_sync_structures();
    };

} // namespace ExhibitEngine
