cmake_minimum_required(VERSION 3.28.3)
PROJECT(epolis)

set(CMAKE_CXX_STANDARD 20)
set(wxBUILD_SHARED OFF)

include(cmake/get_cpm.cmake)
include(cmake/cpm_dependencies.cmake)

file(GLOB_RECURSE sources src/*.cpp)

add_executable(epolis WIN32 ${sources})
target_include_directories(epolis PRIVATE src)
target_link_libraries(epolis PRIVATE wx::core)