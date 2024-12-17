#pragma once
#include "volk.h"
#include "Context.h"
#include "../MemoryAllocation/BumpAllocator.h"
#include "../Logging/Logger.h"
#include <algorithm>


namespace ExhibitEngine{

class SwapChainManager
{
public:
    SwapChainManager(const Window& window, const Context& context);
    ~SwapChainManager();

    void recreateSwapChain(const Context &context);

    const VkRenderPass getRenderPass()const{return renderPass;}
    const VkSwapchainKHR getSwapChain()const{return swapChain;}
    const VkFramebuffer getSwapChainFramebuffer(uint32_t index)const{return swapChainFramebuffers[index];}
    const VkExtent2D getExtent()const{return extent;}
private:
    //derived handles, some might be unnessecary to store as class members
    const HWND windowHandle;
    const VkDevice device;
    
    VkSurfaceCapabilitiesKHR capabilities;
    VkSurfaceFormatKHR surfaceFormat;
    VkPresentModeKHR presentMode;
    VkExtent2D extent;

    //Render pass describes the layout and behavior of framebuffer attachments during a rendering pass.
    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    std::vector<VkImageView> swapChainImageViews;

    VkRenderPass renderPass;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    void querySwapChainSupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
    VkPresentModeKHR chooseSwapPresentMode(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
    VkExtent2D chooseSwapExtent(VkSurfaceCapabilitiesKHR capabilities, VkSurfaceKHR surface);
    
    void createRenderPass();
    void createSwapChain(const Context &context);
    void createImageViews();
    void createFramebuffers();

    void cleanupSwapChain();

};

}