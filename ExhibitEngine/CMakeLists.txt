﻿# CMakeList.txt : CMake project for ExhibitEngine, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_library (ExhibitEngine "src/Application.cpp" "src/Application.h" "src/UI/Window.h" "src/UI/Window.cpp" "src/Renderer/Renderer.cpp" "src/Renderer/Renderer.h" "src/MemoryAllocation/BumpAllocator.h"   "src/Logging/Logger.h" "src/Logging/Logger.cpp" )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ExhibitEngine PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
