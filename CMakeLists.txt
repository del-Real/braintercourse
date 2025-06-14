cmake_minimum_required(VERSION 3.30)
project(braintercourse C)

set(CMAKE_C_STANDARD 11)

# Set the path to raylib installed via Homebrew
set(RAYLIB_PATH "/opt/homebrew/Cellar/raylib/5.5")

# Include raylib headers
include_directories(${RAYLIB_PATH}/include)

# Link raylib library
link_directories(${RAYLIB_PATH}/lib)

# Create the executable from your source files
add_executable(braintercourse main.c)

# Link raylib to your executable
target_link_libraries(braintercourse raylib)