# CMakeList.txt : CMake project for ExhibitEngine, include source and define
# project specific logic here.
#
add_definitions(-DVK_USE_PLATFORM_WIN32_KHR)
add_subdirectory(ThirdParty/volk)
add_subdirectory(ThirdParty/VulkanMemoryAllocator)
add_subdirectory(ThirdParty/glm)
add_subdirectory(ThirdParty/tinyobjloader)

# Add source to this project's executable.
add_library (ExhibitEngine  "src/Application.cpp" "src/Application.h"
                            "src/MemoryAllocation/BumpAllocator.h"
                            "src/Logging/Logger.h" "src/Logging/Logger.cpp"
                            "src/UI/Window.h" "src/UI/Window.cpp"
                            "src/Renderer/Renderer.cpp" "src/Renderer/Renderer.h"
                            "src/Renderer/Context.h" "src/Renderer/Context.cpp"
                            "src/Renderer/SwapChainManager.h" "src/Renderer/SwapChainManager.cpp"
                            "src/Renderer/PipelineManager.h" "src/Renderer/PipelineManager.cpp"
                            "src/Renderer/CommandBufferManager.h" "src/Renderer/CommandBufferManager.cpp")
target_link_libraries(ExhibitEngine PRIVATE volk GPUOpen::VulkanMemoryAllocator glm::glm tinyobjloader)
target_include_directories(ExhibitEngine PRIVATE "ThirdParty/stb")


# TODO: Add tests and install targets if needed.
