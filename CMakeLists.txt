# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.29)
set (CMAKE_CXX_STANDARD 20)

project ("ExhibitEngine")

# Include sub-projects.
add_subdirectory ("ExhibitEngine")
add_subdirectory ("Testbed")

target_link_libraries(Testbed ExhibitEngine)
