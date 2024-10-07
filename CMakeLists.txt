cmake_minimum_required(VERSION 3.25.1)
PROJECT(epolis)

set(CMAKE_CXX_STANDARD 20)
set(wxWidgets_USE_STATIC ON)
set(wxBUILD_SHARED OFF)
set(BUILD_SHARED_LIBS OFF)
set(BUILD_OPENEXR OFF)
set(WITH_FFMPEG OFF)

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")

    if(${MINGW})
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows --static")
    endif()
endif()

if(${MSVC})
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:WinMainCRTStartup")
endif()

include(cmake/get_cpm.cmake)
include(cmake/cpm_dependencies.cmake)

file(GLOB_RECURSE sources src/*.cpp)

add_executable(epolis ${sources})
target_include_directories(epolis PRIVATE
        src
        ${wxWidgets_INCLUDE_DIRS}
        ${opencv_SOURCE_DIR}/include
        ${opencv_SOURCE_DIR}/modules/core/include
        ${opencv_SOURCE_DIR}/modules/imgproc/include
        ${opencv_SOURCE_DIR}/modules/imgcodecs/include
        ${CMAKE_CURRENT_BINARY_DIR}
        ${csv-parser_SOURCE_DIR}/single_include
)
target_link_libraries(epolis PRIVATE wx::core opencv_core opencv_imgproc opencv_imgcodecs)

if(UNIX)
    target_link_libraries(epolis PRIVATE X11)
endif()

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/text.csv DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/input DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)