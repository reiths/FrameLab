cmake_minimum_required (VERSION 4.0)

include(cmake/CompilerWarnings.cmake)
include(cmake/Doxygen.cmake)

# Activate hot reload for MSVC, if supported
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Figure out what depends on what and ensure module interfaces compile before imports
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

project(
	"FrameLab"
	VERSION 0.0.1
	DESCRIPTION ""
	LANGUAGES CXX
)

# Interface library for good defaults shared by all targets.
add_library(project_defaults INTERFACE)

target_compile_features(project_defaults INTERFACE cxx_std_23)

target_compile_options(project_defaults INTERFACE
    $<$<CXX_COMPILER_ID:MSVC>:${MSVC_WARNINGS}>
    $<$<CXX_COMPILER_ID:Clang>:${CLANG_WARNINGS}>
    $<$<CXX_COMPILER_ID:GNU>:${GCC_WARNINGS}>
)

enable_testing()
enable_doxygen()

add_subdirectory("framelab")
add_subdirectory("sandbox")
