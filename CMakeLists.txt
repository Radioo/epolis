cmake_minimum_required(VERSION 3.28.3)
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

if(UNIX)
    find_package(wxWidgets REQUIRED)
    find_package(OpenCV REQUIRED)
endif()

# Set compiler flags
if(UNIX)
    # Run wx-config --cxxflags
    execute_process(COMMAND wx-config --cxxflags OUTPUT_VARIABLE WX_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WX_CXX_FLAGS}")
endif()

file(GLOB_RECURSE sources src/*.cpp)

add_executable(epolis ${sources})
target_include_directories(epolis PRIVATE
        src
        ${wxWidgets_INCLUDE_DIRS}
        ${csv-parser_SOURCE_DIR}/single_include
)
target_link_libraries(epolis PRIVATE ${wxWidgets_LIBRARIES} ${OpenCV_LIBS})

if(NOT UNIX)
    target_include_directories(epolis PRIVATE
            ${opencv_SOURCE_DIR}/include
            ${opencv_SOURCE_DIR}/modules/core/include
            ${opencv_SOURCE_DIR}/modules/imgproc/include
            ${opencv_SOURCE_DIR}/modules/imgcodecs/include
            ${CMAKE_CURRENT_BINARY_DIR}
    )

    target_link_libraries(epolis PRIVATE gcov wx::core opencv_core opencv_imgproc opencv_imgcodecs)
endif()

if(UNIX)
    target_link_libraries(epolis PRIVATE X11)
endif()

if(UNIX)
    set(BIN_DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
else()
    set(BIN_DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)
endif()

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/text.csv DESTINATION ${BIN_DESTINATION})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/input DESTINATION ${BIN_DESTINATION})