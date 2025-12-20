include(Dependencies.cmake)


# Library
add_library(framelab)
add_library(framelab::framelab ALIAS framelab)

target_sources(framelab
    PUBLIC
        FILE_SET framelab_modules TYPE CXX_MODULES BASE_DIRS ./src
        FILES
            "src/framelab.ixx"
            "src/result.ixx"
            "src/logging/component_logger.ixx"
            "src/logging/logging.ixx"
            "src/application/application.ixx"
            "src/application/events.ixx"
            "src/application/imgui.ixx"
            "src/application/layer.ixx"
            "src/application/timestep.ixx"
            "src/application/window.ixx"
            "src/audio/audio.ixx"
    PRIVATE
        #FILE_SET CXX_MODULES FILES
            "src/application/application.cpp"
            "src/application/events.cpp"
            "src/application/window.cpp"
            "src/audio/audio.cpp"
)

target_link_libraries(framelab
    PRIVATE
        project_defaults
        spdlog::spdlog
        Microsoft.GSL::GSL
        whisper
    PUBLIC
        imgui
)