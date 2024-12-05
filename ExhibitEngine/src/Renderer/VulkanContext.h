#pragma once
#ifndef VK_USE_PLATFORM_WIN32_KHR
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#include "volk.h"
#include "../MemoryAllocation/BumpAllocator.h"
#include "../Logging/Logger.h"
#include <vector>
#include <cstring>


#ifdef NDEBUG
#define LAYERNAMES {}
#define EXTENSIONNAMES { VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME}
const bool enableValidationLayers = false;
#else
#define LAYERNAMES {"VK_LAYER_KHRONOS_validation"}
#define EXTENSIONNAMES { VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME, VK_EXT_DEBUG_UTILS_EXTENSION_NAME}
const bool enableValidationLayers = true;
#endif

class VulkanContext {
public:
    void initialize(HINSTANCE instanceHandle, HWND windowHandle);
    void shutDown() const;
    VkDevice getDevice() const { return device; }
    VkSurfaceKHR getSurface() const { return surface; }
    VkQueue getGraphicsQueue() const { return graphicsQueue; }

private:
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkPhysicalDevice physicalDevice;
    std::vector<VkQueueFamilyProperties, BumpAllocator<VkQueueFamilyProperties>> queueFamilies;
    VkDevice device;
    VkSurfaceKHR surface;
    VkQueue graphicsQueue;
    VkQueue presentationQueue;

    std::vector<const char*, BumpAllocator<const char*>> extensions = EXTENSIONNAMES;
    bool checkExtensionSupport();

    std::vector<const char*, BumpAllocator<const char*>> layers = LAYERNAMES;
    bool checkLayerSupport();

    void createInstance();
    
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
    void setupDebugMessenger();
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

    //create surface
    void createSurface(HINSTANCE instanceHandle, HWND windowHandle);


    void pickPhysicalDevice();
	int isDeviceSuitable(VkPhysicalDevice device);
	bool requiredQueuesPresent(VkPhysicalDevice device);
};
