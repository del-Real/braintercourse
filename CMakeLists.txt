cmake_minimum_required(VERSION 3.11)  # FetchContent is available in 3.11+
project(braintercourse C)

set(CMAKE_C_STANDARD 99)

# Generate compile_commands.json for better IDE support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependencies
set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET)  # QUIET or REQUIRED

if (NOT raylib_FOUND)  # If there's none, fetch and build raylib
    include(FetchContent)
    FetchContent_Declare(
            raylib
            DOWNLOAD_EXTRACT_TIMESTAMP OFF
            URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED)  # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_MakeAvailable(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)  # don't build the supplied examples
    endif()
endif()

# Create the executable from your source files
add_executable(braintercourse
        src/interpreter.c
        src/renderer.c
        include/interpreter.h
        src/raygui_impl.c
        include/raygui.h
        include/renderer.h
        include/renderer_config.h
        src/raygui_impl.c
)

# Link raylib to your executable
target_link_libraries(braintercourse raylib)

# Windows-specific configurations
if (WIN32)
    # Link Windows-specific libraries that raylib might need
    target_link_libraries(braintercourse winmm gdi32)

    # MinGW-specific console subsystem flag
    if (MINGW)
        set_target_properties(braintercourse PROPERTIES LINK_FLAGS "-Wl,--subsystem,console")
    endif()
endif()

# macOS frameworks
if (APPLE)
    target_link_libraries(braintercourse "-framework IOKit")
    target_link_libraries(braintercourse "-framework Cocoa")
    target_link_libraries(braintercourse "-framework OpenGL")
endif()