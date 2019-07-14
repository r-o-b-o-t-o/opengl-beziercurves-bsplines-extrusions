cmake_minimum_required(VERSION 3.6)

include_directories(../include)

add_library(
    imgui STATIC
    imconfig.h
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_impl_glfw.cpp
    imgui_impl_glfw.h
    imgui_impl_opengl3.cpp
    imgui_impl_opengl3.h
    imgui_internal.h
    imgui_widgets.cpp
    imgui.cpp
    imgui.h
    imstb_rectpack.h
    imstb_textedit.h
    imstb_truetype.h
)

target_compile_definitions(imgui PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD=1)
