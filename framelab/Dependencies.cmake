include(FetchContent)


FetchContent_Declare(
    gsl
    GIT_REPOSITORY https://github.com/microsoft/GSL
    GIT_TAG v4.2.0
)
FetchContent_MakeAvailable(gsl)


FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.16.0
)
FetchContent_MakeAvailable(spdlog)

# We get quite a lot of warnings from spdlog using -Wall.
# To ignore these warnings, we alter INTERFACE_INCLUDE_DIRECTORIES to be seen as SYSTEM.
get_target_property(_spdlog_inc spdlog::spdlog INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(spdlog SYSTEM INTERFACE ${_spdlog_inc})


find_package(OpenGL REQUIRED)


FetchContent_Declare(
    glad 
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v2.0.8
)
FetchContent_MakeAvailable(glad)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG v1.92.5-docking
)
FetchContent_MakeAvailable(imgui)

# ImGui has no CMake project, so we must create one
FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
    message(FATAL_ERROR "FetchContent failed to populate ImGui")
endif()

add_library(imgui STATIC)

target_sources(imgui
    PRIVATE
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui
    PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends

        # Include the parent folder so we can import "imgui-src/backends.h" instead of "backends.h" for clarity
        #$<BUILD_INTERFACE:${imgui_SOURCE_DIR}/..>
)

target_link_libraries(imgui PUBLIC glfw OpenGL::GL)

target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)


FetchContent_Declare(
    whisper
    GIT_REPOSITORY https://github.com/ggml-org/whisper.cpp
    GIT_TAG v1.8.2
)

set(WHISPER_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(WHISPER_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(WHISPER_BUILD_SERVER OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(whisper)