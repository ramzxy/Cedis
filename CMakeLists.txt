cmake_minimum_required(VERSION 3.15)
project(Cedis VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# This line is critical for Visual Studio IntelliSense when using presets
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find Boost
find_package(Boost REQUIRED COMPONENTS asio)

# Source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
include_directories(include)

add_executable(Cedis ${SOURCES})

# Link Boost
target_link_libraries(Cedis PRIVATE Boost::asio)
