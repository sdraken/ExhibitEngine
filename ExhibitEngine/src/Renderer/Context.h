#pragma once
#include "volk.h"
#include "../MemoryAllocation/BumpAllocator.h"
#include "../Logging/Logger.h"
#include "../UI/Window.h"
#include <vector>
#include <optional>
#include <cstring>
#include <set>



//predefines for extensions and validation layers depending on debug
#ifdef NDEBUG
#define LAYERNAMES
#define EXTENSIONNAMES VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME
#define ENABLEVALIDATINLAYERS false
#else
#define LAYERNAMES {"VK_LAYER_KHRONOS_validation"}
#define EXTENSIONNAMES { VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME, VK_EXT_DEBUG_UTILS_EXTENSION_NAME}
#define ENABLEVALIDATINLAYERS true
#endif

#define DEVICEEXTENSIONNAMES {VK_KHR_SWAPCHAIN_EXTENSION_NAME}

namespace ExhibitEngine {
struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentationFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentationFamily.has_value();
    }
};



class Context {
public:

    Context(const Window& window);
    ~Context();

    const VkPhysicalDevice getPhysicalDevice()const{return physicalDevice;}
    const VkDevice getDevice()const{return device;}
    const VkSurfaceKHR getSurface()const{return surface;}
    const VkQueue getGraphicsQueue()const{return graphicsQueue;}
    const uint32_t getGraphicsQueueIndex()const{return queueFamilyIndices.graphicsFamily.value();}
    const VkQueue getPresentationQueue()const{return presentationQueue;}
    const uint32_t getPresentationQueueIndex()const{return queueFamilyIndices.presentationFamily.value();}

    
private:
    //List of extension/validation names
    std::vector<const char*> extensions = EXTENSIONNAMES;
    std::vector<const char*> layers = LAYERNAMES;
    std::vector<const char*> deviceExtensions = DEVICEEXTENSIONNAMES;

    //handles
    VkInstance instance;                                //instance
    VkDebugUtilsMessengerEXT debugMessenger;            //debug messenger
    VkSurfaceKHR surface;                               //surface
    VkPhysicalDevice physicalDevice;                    //physical device
    VkDevice device;                                    //logical device
    VkQueue graphicsQueue;                              //graphics queue
    VkQueue presentationQueue;                          //presentation queue
    QueueFamilyIndices queueFamilyIndices;


    //instance
    void createInstance();
    bool checkExtensionSupport();
    bool checkLayerSupport();

    //Validation layer
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
    void setupDebugMessenger();
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
  
    //Surface
    void createSurface(HINSTANCE instanceHandle, HWND windowHandle);

    //physical device
    void pickPhysicalDevice();
    int isDeviceSuitable(VkPhysicalDevice physicalDevice);
    bool checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice);
    QueueFamilyIndices getQueueFamilyIndices(VkPhysicalDevice physicalDevice);

    void createLogicalDevice();

};

}