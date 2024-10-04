cmake_minimum_required(VERSION 3.28.3)
PROJECT(epolis)

set(CMAKE_CXX_STANDARD 20)
set(wxBUILD_SHARED OFF)
set(BUILD_SHARED_LIBS OFF)

if(${MSVC})
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:WinMainCRTStartup")
endif()

include(cmake/get_cpm.cmake)
include(cmake/cpm_dependencies.cmake)

file(GLOB_RECURSE sources src/*.cpp)

add_executable(epolis ${sources})
target_include_directories(epolis PRIVATE
        src
        ${opencv_SOURCE_DIR}/include
        ${opencv_SOURCE_DIR}/modules/core/include
        ${opencv_SOURCE_DIR}/modules/imgproc/include
        ${opencv_SOURCE_DIR}/modules/imgcodecs/include
        ${CMAKE_CURRENT_BINARY_DIR}
        ${csv-parser_SOURCE_DIR}/single_include
)
target_link_libraries(epolis PRIVATE gcov wx::core opencv_core opencv_imgproc opencv_imgcodecs)

if(UNIX)
    target_link_libraries(epolis PRIVATE X11 gcov)
endif()

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/text.csv DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)