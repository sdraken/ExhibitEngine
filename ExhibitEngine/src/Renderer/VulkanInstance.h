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

#include "VulkanDevice.h"
#include "../MemoryAllocation/BumpAllocator.h"
#include "../Logging/Logger.h"



#ifdef NDEBUG
#define LAYERNAMES {}
#define EXTENSIONNAMES { VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME}
const bool enableValidationLayers = false;
#else
#define LAYERNAMES {"VK_LAYER_KHRONOS_validation"}
#define EXTENSIONNAMES { VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME, VK_EXT_DEBUG_UTILS_EXTENSION_NAME}
const bool enableValidationLayers = true;
#endif

namespace ExhibitEngine {
    const char* extensions[];
    const char* validationLayers[];

    class VulkanInstance {
    public:
        void initilize(HINSTANCE instanceHandle, HWND windowHandle);

        void shutDown() const;

    private:
        VkInstance instance;
        VkDebugUtilsMessengerEXT debugMessenger;
        VkSurfaceKHR surface;
        VulkanDevice device;

        std::vector<const char*, BumpAllocator<const char*>> extensions = EXTENSIONNAMES;
        std::vector<const char*, BumpAllocator<const char*>> layers = LAYERNAMES;

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData);
        
        //Vulkan instance
        void createInstance();
        
        //Check if extensions/layers are supported
        bool checkExtensionSupport();
        bool checkLayerSupport();

        //Validation layers
        void setupDebugMessenger();
        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

        //create surface
        void createSurface(HINSTANCE instanceHandle, HWND windowHandle);


        void createDevice();

    };

}
