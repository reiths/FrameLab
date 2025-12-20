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