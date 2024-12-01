#include "VulkanDevice.h"
#include <inttypes.h>
namespace ExhibitEngine{
void VulkanDevice::initilize(std::vector <VkPhysicalDevice, BumpAllocator<VkPhysicalDevice>> const& devices)
{
    pickPhysicalDevice(devices);
}

void VulkanDevice::shutDown()
{
}


void VulkanDevice::pickPhysicalDevice(std::vector <VkPhysicalDevice, BumpAllocator<VkPhysicalDevice>> const &devices)
{

    int highestDeviceScore = 0;
    LOGDEBUG("LOOKING FOR THE BEST VULKAN DEVICE");
    for (const auto& device : devices) {
        int currentDeviceScore = isDeviceSuitable(device);
        LOGDEBUG("DEVICE SCORED: %d", currentDeviceScore);
        if (currentDeviceScore > highestDeviceScore) {

            highestDeviceScore = currentDeviceScore;
            physicalDevice = device;
        }
    }

    if (highestDeviceScore == 0) {
        LOGFATAL("failed to find a suitable GPU!");
    }
}

int VulkanDevice::isDeviceSuitable(VkPhysicalDevice device)
{
    int score = 0;

    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);

    LOGDEBUG("FOUND DEVICE: %s", deviceProperties.deviceName);

    // Discrete GPUs have a significant performance advantage
    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 1000;
    }

    // Maximum possible size of textures affects graphics quality
    score += deviceProperties.limits.maxImageDimension2D;

    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    // Application can't function without geometry shaders
    if (!deviceFeatures.geometryShader) {
        return 0;
    }

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    LOGDEBUG("CHECKING QUEUE FAMILES");
    for (VkQueueFamilyProperties const& queueFamily : queueFamilies) {
        LOGDEBUG("NEW QUEUE FAMILY: %d queues with %d (%d, %d, %d) %d", queueFamily.queueCount, queueFamily.queueFlags, queueFamily.minImageTransferGranularity.width, queueFamily.minImageTransferGranularity.height, queueFamily.minImageTransferGranularity.depth, queueFamily.timestampValidBits);
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            LOGDEBUG("SUPPORTS GRAPHICS OPERATIONS");
            graphicsQueueIndices.push_back(i);
        }
        if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) {
            LOGDEBUG("SUPPORTS COMPUTE OPERATIONS");
            computeQueueIndices.push_back(i);
        } 
        if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) {
            LOGDEBUG("SUPPORTS TRANSFER OPERATIONS");
            transferQueueIndices.push_back(i);
        }
        LOGDEBUG("%p", &graphicsQueueIndices[0]);
        LOGDEBUG("%p", &computeQueueIndices[0]);
        LOGDEBUG("%p", &transferQueueIndices[0]);

        i++;
    }

    LOGDEBUG("GRAPHICS QUEUE INDICES");
    for (uint32_t index : graphicsQueueIndices) {
        LOGDEBUG("%d", index);
    }
    LOGDEBUG("COMPUTE QUEUE INDICES");
    for (uint32_t index : computeQueueIndices) {
        LOGDEBUG("%d", index);
    }
    LOGDEBUG("TRANSFER QUEUE INDICES");
    for (uint32_t index : transferQueueIndices) {
        LOGDEBUG("%d", index);
    }

    return score;
}

}