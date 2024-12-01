

#pragma once
#include <volk/volk.h>
#include "../MemoryAllocation/BumpAllocator.h"
#include "../Logging/Logger.h"
#include <vector>


namespace ExhibitEngine {

	struct QueueFamilyIndices {
		VkQueueFlagBits foundFamilies;

		//store indicies aswell, right now only storing bitfield telling us what Queue families exist
	};

	class VulkanDevice
	{
	public:

		void initilize(std::vector<VkPhysicalDevice, BumpAllocator<VkPhysicalDevice>> const& devices);
		void shutDown();

	private:
		VkPhysicalDevice physicalDevice;
		VkSurfaceKHR surface;	
		std::vector<uint32_t, BumpAllocator<uint32_t>> graphicsQueueIndices;
		std::vector<uint32_t, BumpAllocator<uint32_t>> computeQueueIndices;
		std::vector<uint32_t, BumpAllocator<uint32_t>> transferQueueIndices;


		void pickPhysicalDevice(std::vector<VkPhysicalDevice, BumpAllocator<VkPhysicalDevice>> const& devices);
		int isDeviceSuitable(VkPhysicalDevice device);
	};

}