cmake_minimum_required(VERSION 3.16)
project(Cedis LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # Helpful for IDEs

# Enable folders in Visual Studio for better organization
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Optional: vcpkg integration (uncomment if using vcpkg)
# set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

# Find Boost (system is required for asio error codes)
find_package(Boost REQUIRED COMPONENTS system)

# Include Boost headers
include_directories(${Boost_INCLUDE_DIRS})

# Add include directory for your own headers (optional)
include_directories(${PROJECT_SOURCE_DIR}/include)

# Collect source files
file(GLOB_RECURSE SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    ${PROJECT_SOURCE_DIR}/main.cpp
)

# Create the executable
add_executable(Cedis ${SOURCES})

# Link Boost.System (required by Asio)
target_link_libraries(Cedis PRIVATE Boost::system)