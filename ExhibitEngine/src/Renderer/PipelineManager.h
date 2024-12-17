#pragma once
#include "Context.h"
#include "SwapChainManager.h"
#include <string>
#include <fstream>

namespace ExhibitEngine{


    class PipelineManager
    {
    public:
        PipelineManager(const Context& context, const SwapChainManager& swapChainManager);
        ~PipelineManager();

        const VkPipeline getGraphicsPipeline()const{return graphicsPipeline;}
    private:
        const VkDevice device;
        
        //Pipeline layout describes the interface between shaders and resources, such as descriptors and push constants.
        VkPipelineLayout pipelineLayout;
        //represents a fully configured rendering pipeline
        VkPipeline graphicsPipeline;

        void createGraphicsPipeline(VkRenderPass renderPass);
        static std::vector<char> readFile(const std::string& filename);
        VkShaderModule createShaderModule(const std::vector<char>& code);
    };

}



