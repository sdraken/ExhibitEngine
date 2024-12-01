//	Description: Implementation of Renderer class
//
//	Author: Svante Drakenberg

#include "VulkanInstance.h"



namespace ExhibitEngine {
    

    void VulkanInstance::initilize(HINSTANCE instanceHandle, HWND windowHandle)
    {
        LOGDEBUG("%d", _MSVC_LANG);
        createInstance();
        setupDebugMessenger();
        createSurface(instanceHandle, windowHandle);
        createDevice();
    }

    void VulkanInstance::shutDown() const
    {
        if (enableValidationLayers) {
            vkDestroyDebugUtilsMessengerEXT(instance, debugMessenger, NULL);
        }
        vkDestroyInstance(instance, NULL);
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL VulkanInstance::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
    {
        LOGDEBUG("validation layer: %s", pCallbackData->pMessage);

        return VK_FALSE;
        
    }

    void VulkanInstance::createInstance(){

        //initalize volk
        if (volkInitialize() != VK_SUCCESS) LOGFATAL("Failed to initalize volk");
        
        //if debugging, check if layer exists
        if (enableValidationLayers && !checkLayerSupport()) LOGFATAL("validation layers requested, but not available!");
        
        //check if extensions exists
        if (!checkExtensionSupport()) LOGFATAL("REQUIRED EXTENSION NOT FOUND");

        //Optional information about application
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;         //sType
        appInfo.pNext = NULL;                                       //pNext
        appInfo.pApplicationName = "Testbed";                       //pApplicationName
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);      //applicationVersion
        appInfo.pEngineName = "ExhibitEngine";                      //pEngineName
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);           //engineVersion
        appInfo.apiVersion = VK_API_VERSION_1_0;                    //apiVersion



        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;                  //sType
        createInfo.pNext = NULL;                                                    //pNext
        createInfo.flags = NULL;                                                    //flags
        createInfo.pApplicationInfo = &appInfo;                                     //pApplicationInfo
        createInfo.enabledLayerCount = layers.size();                               //enabledLayerCount
        createInfo.ppEnabledLayerNames = layers.data();                             //ppEnabledLayerNames
        createInfo.enabledExtensionCount = extensions.size();                       //enabledExtensionCount  
        createInfo.ppEnabledExtensionNames = extensions.data();                     //ppEnabledExtensionNames


        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        if (enableValidationLayers) {
            populateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
        }


        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            LOGFATAL("failed to create instance!");
        }

        volkLoadInstance(instance);

    }

    bool VulkanInstance::checkExtensionSupport()
    {
        uint32_t extensionCount;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());

        //LOGDEBUG("Found %d extensions", extensionCount);
        //for (VkExtensionProperties extension : availableExtensions) {
        //    LOGDEBUG("%s", extension.extensionName);
        //}

        LOGDEBUG("CHECKING FOR ALL REQUESTED EXTENSIONS");
        for (const char* extensionName : extensions) {
            LOGDEBUG("TRYING TO FIND EXTENSION %s", extensionName);
            bool extensionFound = false;

            for (const auto& extensionProperties : availableExtensions) {
                if (strcmp(extensionName, extensionProperties.extensionName) == 0) {
                    extensionFound = true;
                    break;
                }
            }

            if (!extensionFound) {
                return false;
            }
            LOGDEBUG("FOUND EXTENSION %s", extensionName);
        }
        LOGDEBUG("FOUND ALL REQUESTED EXTENSIONS.");
        return true;
    }

    bool VulkanInstance::checkLayerSupport()
    {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        //LOGDEBUG("Found %d extensions", layerCount);
        //for (VkLayerProperties layer : availableLayers) {
        //    LOGDEBUG("%s", layer.layerName);
        //}

        LOGDEBUG("CHECKING FOR ALL REQUESTED LAYERS");
        for (const char* layerName : layers) {
            LOGDEBUG("TRYING TO FIND LAYER %s", layerName);
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
            LOGDEBUG("FOUND LAYER %s", layerName);
        }
        LOGDEBUG("FOUND ALL REQUESTED LAYERS.");
        return true;
    }

    void VulkanInstance::setupDebugMessenger()
    {
        if (!enableValidationLayers) {
            return;
        }
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
        populateDebugMessengerCreateInfo(debugCreateInfo);

        if (vkCreateDebugUtilsMessengerEXT(instance, &debugCreateInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
            LOGFATAL("failed to create instance!");
        }

    }

    void VulkanInstance::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& debugCreateInfo)
    {
        debugCreateInfo = {};
        debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debugCreateInfo.pfnUserCallback = debugCallback;
        debugCreateInfo.pUserData = nullptr;
    }

    void VulkanInstance::createSurface(HINSTANCE instanceHandle, HWND windowHandle)
    {

        VkWin32SurfaceCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        createInfo.hinstance = instanceHandle;
        createInfo.hwnd = windowHandle;
        
        //if (vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface) != VK_SUCCESS) {
        //    LOGFATAL("failed to create surface!");
        //}
    }

    void VulkanInstance::createDevice()
    {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        if (deviceCount == 0) {
            LOGFATAL("failed to find GPUs with Vulkan support!");
        }

        std::vector<VkPhysicalDevice, BumpAllocator<VkPhysicalDevice>> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
        
        
        device.initilize(devices);
    }

    

   







}




