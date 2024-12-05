# CMakeList.txt : CMake project for ExhibitEngine, include source and define
# project specific logic here.
#
set(VOLK_STATIC_DEFINES VK_USE_PLATFORM_WIN32_KHR)
add_subdirectory(volk)

# Add source to this project's executable.
add_library (ExhibitEngine "src/Application.cpp" "src/Application.h" "src/UI/Window.h" "src/UI/Window.cpp" "src/Renderer/Renderer.cpp" "src/Renderer/Renderer.h" "src/MemoryAllocation/BumpAllocator.h"   "src/Logging/Logger.h" "src/Logging/Logger.cpp"  "src/Renderer/VulkanContext.h" "src/Renderer/VulkanContext.cpp")
target_link_libraries(ExhibitEngine PRIVATE volk)


# TODO: Add tests and install targets if needed.
