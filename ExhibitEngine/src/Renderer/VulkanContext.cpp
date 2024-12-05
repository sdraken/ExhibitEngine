#include "VulkanContext.h"

void VulkanContext::initialize(HINSTANCE instanceHandle, HWND windowHandle)
{
        createInstance();
        setupDebugMessenger();
        createSurface(instanceHandle, windowHandle);
        pickPhysicalDevice();
}

void VulkanContext::shutDown() const
{
        if (enableValidationLayers) {
            vkDestroyDebugUtilsMessengerEXT(instance, debugMessenger, NULL);
        }
        vkDestroySurfaceKHR(instance, surface, nullptr);
        vkDestroyInstance(instance, nullptr);
}

void VulkanContext::createInstance(){

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
    createInfo.flags = 0;                                                    //flags
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

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanContext::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData)
{
    LOGDEBUG("validation layer: %s", pCallbackData->pMessage);

    return VK_FALSE;
}

void VulkanContext::setupDebugMessenger()
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

void VulkanContext::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &debugCreateInfo)
{
    debugCreateInfo = {};
    debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debugCreateInfo.pfnUserCallback = debugCallback;
    debugCreateInfo.pUserData = nullptr;
}

void VulkanContext::createSurface(HINSTANCE instanceHandle, HWND windowHandle)
{
    VkWin32SurfaceCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.hinstance = instanceHandle;
    createInfo.hwnd = windowHandle;
    
    if (vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface) != VK_SUCCESS) {
        LOGFATAL("failed to create surface!");
    }
}

void VulkanContext::pickPhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        LOGFATAL("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice, BumpAllocator<VkPhysicalDevice>> physicalDevices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, physicalDevices.data());

    int highestDeviceScore = 0;
    LOGDEBUG("LOOKING FOR THE BEST VULKAN DEVICE");
    for (const auto& physicalDevice : physicalDevices) {
        int currentDeviceScore = isDeviceSuitable(physicalDevice);
        LOGDEBUG("DEVICE SCORED: %d", currentDeviceScore);
        if (currentDeviceScore > highestDeviceScore) {

            highestDeviceScore = currentDeviceScore;
            this->physicalDevice = physicalDevice;
        }
    }

    if (highestDeviceScore == 0) {
        LOGFATAL("failed to find a suitable GPU!");
    }
}

int VulkanContext::isDeviceSuitable(VkPhysicalDevice physicalDevice)
{
    int score = 0;

    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

    LOGDEBUG("FOUND DEVICE: %s", deviceProperties.deviceName);

    // Discrete GPUs have a significant performance advantage
    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 1000;
    }

    // Maximum possible size of textures affects graphics quality
    score += deviceProperties.limits.maxImageDimension2D;

    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);

    // Application can't function without geometry shaders
    if (!deviceFeatures.geometryShader) {
        return 0;
    }

    if (!requiredQueuesPresent(physicalDevice)) return 0;

    return score;
}

bool VulkanContext::requiredQueuesPresent(VkPhysicalDevice device)
{
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    LOGDEBUG("NUMBER OF QUEUE FAMILES: %d", queueFamilyCount);

    std::vector<VkQueueFamilyProperties, BumpAllocator<VkQueueFamilyProperties>> queueFamiliesTEMP(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamiliesTEMP.data());


    bool foundGraphicsQueue = false;
    bool foundPresentationQueue = false;

    
    LOGDEBUG("NUMBER OF QUEUE FAMILES: %d", queueFamiliesTEMP.size());
    
    for(int i = 0; i<queueFamiliesTEMP.size(); i++){
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

        LOGDEBUG("queuefamily %d supports:", i);
        if (presentSupport) {
            LOGDEBUG("Presentation");
            foundPresentationQueue = true;
        }

        if (queueFamiliesTEMP[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            LOGDEBUG("Graphics");
            foundGraphicsQueue = true;
        }
        
        if (queueFamiliesTEMP[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
            LOGDEBUG("Compute");
        }
        
        if (queueFamiliesTEMP[i].queueFlags & VK_QUEUE_TRANSFER_BIT) {
            LOGDEBUG("TRANSFER");
        }

        if (queueFamiliesTEMP[i].queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) {
            LOGDEBUG("SPARSE BINDING");
        }


    }

    if(foundPresentationQueue && foundGraphicsQueue) return true;

    return false;
}

bool VulkanContext::checkExtensionSupport()
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

bool VulkanContext::checkLayerSupport()
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

