//	Description: Implementation of Renderer class
//
//	Author: Svante Drakenberg

#include "Renderer.h"



namespace ExhibitEngine {
    
    const char* extensions[] = {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME
    };


    void Renderer::initilize()
    {
        createInstance();
    }

    void Renderer::shutDown()
    {
        //vkDestroyInstance(instance, NULL);
    }

    void Renderer::createInstance(){

        if (volkInitialize() != VK_SUCCESS) {
            LOGFATAL("Failed to initalize volk");
        }


        //Optional information about application
        VkApplicationInfo appInfo{
            VK_STRUCTURE_TYPE_APPLICATION_INFO,     //sType
            NULL,                                   //pNext
            "Testbed",                              //pApplicationName
            VK_MAKE_VERSION(1,0,0),                 //applicationVersion
            "ExhibitEngine",                        //pEngineName
            VK_MAKE_VERSION(1, 0, 0),               //engineVersion
            VK_API_VERSION_1_0                      //apiVersion
        };


        //PRINT ALL AVAILABLE EXTENSIONS
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties, BumpAllocator<VkExtensionProperties>> extensionsVECTOR(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensionsVECTOR.data());

        LOGDEBUG("Found %d extensions", extensionCount);
        for (VkExtensionProperties extension : extensionsVECTOR) {
            LOGDEBUG("%s", extension.extensionName);
        }
     
        VkInstanceCreateInfo createInfo{
            VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,     //sType
            NULL,                                       //pNext
            NULL,                                       //flags
            &appInfo,                                   //pApplicationInfo
            0,                                          //enabledLayerCount
            NULL,                                       //ppEnabledLayerNames
            sizeof(extensions) / sizeof(char*),         //enabledExtensionCount
            extensions                                  //ppEnabledExtensionNames
            };

        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            LOGFATAL("failed to create instance!");
        }

        volkLoadInstance(instance);
    }

}




