#pragma once
#include "../Logging/Logger.h"
#include "Context.h"
#include "SwapChainManager.h"
#include "PipelineManager.h"
#define MAX_FRAMES_IN_FLIGHT 2

namespace ExhibitEngine{


    class CommandBufferManager
    {
    public:
        CommandBufferManager(const Context& context);
        ~CommandBufferManager();

        const VkCommandBuffer getCommandBuffer(uint32_t index)const{return commandBuffers[index];}

        void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, const SwapChainManager& swapChainManager, const PipelineManager& pipelineManager);
    private:
        const VkDevice device;
        const uint32_t graphicsQueueIndex;

        
        VkCommandPool commandPool;
        std::vector<VkCommandBuffer> commandBuffers;

        void createCommandPool();
        void createCommandBuffer();

    };


}