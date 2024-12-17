#include "Context.h"

ExhibitEngine::Context::Context(const Window& window)
{
    createInstance();
    setupDebugMessenger();
    createSurface(window.getHINSTANCE(), window.getHWND());
    pickPhysicalDevice();
    createLogicalDevice();
}

ExhibitEngine::Context::~Context() 
{
    LOGDEBUG("Context deconstructor");
    vkDestroyDevice(device, nullptr);
    if (ENABLEVALIDATINLAYERS) {
        vkDestroyDebugUtilsMessengerEXT(instance, debugMessenger, NULL);
    }
    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);
}

void ExhibitEngine::Context::createInstance(){

    //initalize volk
    if (volkInitialize() != VK_SUCCESS) LOGFATAL("Failed to initalize volk");
    
    //if debugging, check if layer exists
    if (ENABLEVALIDATINLAYERS && !checkLayerSupport()) LOGFATAL("validation layers requested, but not available!");
    
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
    createInfo.enabledLayerCount = layers.size();                             //enabledLayerCount
    createInfo.ppEnabledLayerNames = layers.data();                             //ppEnabledLayerNames
    createInfo.enabledExtensionCount = extensions.size();                       //enabledExtensionCount  
    createInfo.ppEnabledExtensionNames = extensions.data();                     //ppEnabledExtensionNames


    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (ENABLEVALIDATINLAYERS) {
        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    }


    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        LOGFATAL("failed to create instance!");
    }

    volkLoadInstance(instance);

    LOGDEBUG("Created instance");

}

VKAPI_ATTR VkBool32 VKAPI_CALL ExhibitEngine::Context::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData)
{
    LOGDEBUG("validation layer: %s", pCallbackData->pMessage);

    return VK_FALSE;
}

void ExhibitEngine::Context::setupDebugMessenger()
{
        if (!ENABLEVALIDATINLAYERS) {
        return;
    }
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
    populateDebugMessengerCreateInfo(debugCreateInfo);

    if (vkCreateDebugUtilsMessengerEXT(instance, &debugCreateInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
        LOGFATAL("failed to create instance!");
    }
    LOGDEBUG("Created debug messenger");
}

void ExhibitEngine::Context::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &debugCreateInfo)
{
    debugCreateInfo = {};
    debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debugCreateInfo.pfnUserCallback = debugCallback;
    debugCreateInfo.pUserData = nullptr;
}

bool ExhibitEngine::Context::checkExtensionSupport()
{
    uint32_t extensionCount;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());

    //LOGDEBUG("Found %d extensions", extensionCount);
    //for (VkExtensionProperties extension : availableExtensions) {
    //    LOGDEBUG("%s", extension.extensionName);
    //}

    //LOGDEBUG("CHECKING FOR ALL REQUESTED EXTENSIONS");
    for (const char* extensionName : extensions) {
        //LOGDEBUG("TRYING TO FIND EXTENSION %s", extensionName);
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
        //LOGDEBUG("FOUND EXTENSION %s", extensionName);
    }
    //LOGDEBUG("FOUND ALL REQUESTED EXTENSIONS.");
    return true;
}

bool ExhibitEngine::Context::checkLayerSupport()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    //LOGDEBUG("Found %d extensions", layerCount);
    //for (VkLayerProperties layer : availableLayers) {
    //    LOGDEBUG("%s", layer.layerName);
    //}

    //LOGDEBUG("CHECKING FOR ALL REQUESTED LAYERS");
    for (const char* layerName : layers) {
        //LOGDEBUG("TRYING TO FIND LAYER %s", layerName);
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
        //LOGDEBUG("FOUND LAYER %s", layerName);
    }
    //LOGDEBUG("FOUND ALL REQUESTED LAYERS.");
    return true;
}

void ExhibitEngine::Context::createSurface(HINSTANCE instanceHandle, HWND windowHandle)
{
    VkWin32SurfaceCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.hinstance = instanceHandle;
    createInfo.hwnd = windowHandle;
    
    if (vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface) != VK_SUCCESS) {
        LOGFATAL("failed to create surface!");
    }
    LOGDEBUG("Created surface");
}

void ExhibitEngine::Context::pickPhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        LOGFATAL("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, physicalDevices.data());

    int highestDeviceScore = 0;
    //LOGDEBUG("LOOKING FOR THE BEST VULKAN DEVICE");
    for (const auto& physicalDeviceTemp : physicalDevices) {
        int currentDeviceScore = isDeviceSuitable(physicalDeviceTemp);
        //LOGDEBUG("DEVICE SCORED: %d", currentDeviceScore);
        if (currentDeviceScore > highestDeviceScore) {
            QueueFamilyIndices queueFamilyIndicesTemp = getQueueFamilyIndices(physicalDeviceTemp);
            uint32_t formatCount;
            vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDeviceTemp, surface, &formatCount, nullptr);
            uint32_t presentModeCount;
            vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDeviceTemp, surface, &presentModeCount, nullptr);

            if(queueFamilyIndicesTemp.isComplete() && formatCount!=0 && presentModeCount != 0){
                highestDeviceScore = currentDeviceScore;
                physicalDevice = physicalDeviceTemp;

                queueFamilyIndices = queueFamilyIndicesTemp;
            }


        }
    }

    if (highestDeviceScore == 0) {
        LOGFATAL("failed to find a suitable GPU!");
    }

    LOGDEBUG("Physical device found");
}

int ExhibitEngine::Context::isDeviceSuitable(VkPhysicalDevice physicalDevice)
{
    int score = 0;

    //DEVICE PROPERTIES
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

    //LOGDEBUG("FOUND DEVICE: %s", deviceProperties.deviceName);

    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 1000;
    }

    score += deviceProperties.limits.maxImageDimension2D;

    //DEVICE FEATURES
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);

    //Requierments
    if(checkDeviceExtensionSupport(physicalDevice) && deviceFeatures.geometryShader) return score;

    return 0;
}

bool ExhibitEngine::Context::checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

    //LOGDEBUG("CHECKING FOR ALL REQUESTED EXTENSIONS");
    for (const char* deviceExtensionName : deviceExtensions) {
        //LOGDEBUG("TRYING TO FIND EXTENSION %s", deviceExtensionName);
        bool extensionFound = false;

        for (const auto& extensionProperties : availableExtensions) {
            if (strcmp(deviceExtensionName, extensionProperties.extensionName) == 0) {
                extensionFound = true;
                break;
            }
        }

        if (!extensionFound) {
            return false;
        }
        //LOGDEBUG("FOUND EXTENSION %s", deviceExtensionName);
    }
    //LOGDEBUG("FOUND ALL REQUESTED EXTENSIONS.");
    return true;
}

ExhibitEngine::QueueFamilyIndices ExhibitEngine::Context::getQueueFamilyIndices(VkPhysicalDevice physicalDevice)
{
    QueueFamilyIndices queueFamilyIndices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

    //LOGDEBUG("NUMBER OF QUEUE FAMILES: %d", queueFamilies.size());
    for(int i = 0; i<queueFamilies.size(); i++){
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);


        //LOGDEBUG("queuefamily %d has %d queues and support:", i, queueFamilies[i].queueCount );
        if (presentSupport) {
            //LOGDEBUG("Presentation");
            queueFamilyIndices.presentationFamily = i;
        }

        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            //LOGDEBUG("Graphics");
            queueFamilyIndices.graphicsFamily = i;
        }

    }



    return queueFamilyIndices;
}



void ExhibitEngine::Context::createLogicalDevice()
{
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {queueFamilyIndices.graphicsFamily.value(), queueFamilyIndices.presentationFamily.value()};

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }
        
        //to be filled later
        VkPhysicalDeviceFeatures deviceFeatures{};

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        
        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.enabledExtensionCount = deviceExtensions.size();
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        //legacy
        if (ENABLEVALIDATINLAYERS) {
            createInfo.enabledLayerCount = layers.size();                             //enabledLayerCount
            createInfo.ppEnabledLayerNames = layers.data();   
        } else {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
            LOGFATAL("failed to create logical device!");
        }
        LOGDEBUG("Created logical device");
        vkGetDeviceQueue(device, queueFamilyIndices.graphicsFamily.value(), 0, &graphicsQueue);
        LOGDEBUG("Graphics queue obtained");
        vkGetDeviceQueue(device, queueFamilyIndices.presentationFamily.value(), 0, &presentationQueue);
        LOGDEBUG("Presentation queue obtained");
}
