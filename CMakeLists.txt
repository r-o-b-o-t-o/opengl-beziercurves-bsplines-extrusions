cmake_minimum_required(VERSION 3.6)
project(MathsOpenGL)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# GLFW
find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

if (APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(OpenGL_LIBRARY OpenGL)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREVIDEO_LIBRARY CoreVideo)
    set(EXTRA_LIBS ${COCOA_LIBRARY} ${OpenGL_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY})
endif (APPLE)

add_subdirectory(imgui)
include_directories(include .)
add_executable(
    mathsopengl

    "src/main.cpp"
    "src/glad.c"

    "src/shader.cpp"
    "src/orbit_camera.cpp"
    "src/point.cpp"
    "src/casteljau.cpp"
)
target_link_libraries(mathsopengl imgui ${OPENGL_LIBRARIES} ${GLFW_LIBRARY_DIRS} ${EXTRA_LIBS} dl glfw)

# Find shader files
set(shaders_dir ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
file(GLOB_RECURSE shaders "${shaders_dir}/*")

# Copy shaders to target location
foreach (file ${shaders})
    if (NOT IS_DIRECTORY ${file})
        STRING(REGEX REPLACE "^${shaders_dir}/" "" file_dest_name ${file})
        configure_file(${file} ${CMAKE_CURRENT_BINARY_DIR}/shaders/${file_dest_name} COPYONLY)
    endif ()
endforeach ()
