add_executable(sandbox)

target_sources(sandbox
    PRIVATE
        FILE_SET sandbox_modules TYPE CXX_MODULES BASE_DIRS ./src
        FILES
            "src/layers.ixx"
            
    PRIVATE
        "src/main.cpp"
        "src/layers.cpp"
)


target_link_libraries(sandbox
    PRIVATE
        project_defaults
        framelab::framelab
)

target_compile_options(sandbox
	PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/wd5050> # Having to manually disable C5050 is probably a compiler error
)