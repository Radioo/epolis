cmake_minimum_required(VERSION 3.25.1)
PROJECT(epolis)

set(CMAKE_CXX_STANDARD 20)

include(cmake/get_cpm.cmake)
include(cmake/cpm_dependencies.cmake)

find_package(wxWidgets CONFIG REQUIRED)
find_package(OpenCV CONFIG REQUIRED)

file(GLOB_RECURSE sources src/*.cpp)

if(WIN32)
    add_executable(epolis WIN32 ${sources})
else()
    add_executable(epolis ${sources})
endif()
target_include_directories(epolis PRIVATE
        src
        ${csv-parser_SOURCE_DIR}/single_include
)
target_link_libraries(epolis PRIVATE wx::core opencv_core opencv_imgproc opencv_imgcodecs)

if(WIN32)
    target_sources(epolis PRIVATE epolis.manifest)
endif()

if(UNIX)
    target_link_libraries(epolis PRIVATE X11)
endif()

set(CONFIGURATIONS Debug Release)

foreach(CONFIG ${CONFIGURATIONS})
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/text.csv DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${CONFIG})
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/input DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${CONFIG})
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/images DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${CONFIG})
endforeach()
