cmake_minimum_required(VERSION 3.20)
project(Cedis)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set policy for modern Boost finding
if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

# Enable vcpkg integration if available
if(DEFINED CMAKE_TOOLCHAIN_FILE)
    message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
endif()

# macOS-specific setup for Homebrew Boost
if(APPLE)
    # Detect Homebrew installation path
    if(CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "arm64")
        # Apple Silicon Macs
        set(HOMEBREW_PREFIX "/opt/homebrew")
    else()
        # Intel Macs
        set(HOMEBREW_PREFIX "/usr/local")
    endif()
    
    # Add Homebrew paths to help find Boost
    if(EXISTS "${HOMEBREW_PREFIX}")
        list(APPEND CMAKE_PREFIX_PATH "${HOMEBREW_PREFIX}")
        message(STATUS "Added Homebrew prefix to search path: ${HOMEBREW_PREFIX}")
    endif()
endif()

# Find Boost using CONFIG mode (works best with vcpkg)
find_package(Boost REQUIRED CONFIG COMPONENTS 
    system 
    thread
)

# Add executable with correct source paths
add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/connection.cpp
    src/parser.cpp
        src/handleCMD.cpp
        include/commands/echo.h
)

# Include directories - add the include folder for headers
target_include_directories(${PROJECT_NAME} PRIVATE 
    include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link Boost libraries using modern target-based approach
target_link_libraries(${PROJECT_NAME} PRIVATE
    Boost::system
    Boost::thread
)

# Add compiler definitions for debug mode if building in debug
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG_MODE)
endif()

# For Windows, ensure proper linking and definitions
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE 
        _WIN32_WINNT=0x0601  # Windows 7 or later for ASIO
        WIN32_LEAN_AND_MEAN
        NOMINMAX
    )
    
    # Link Windows socket libraries for ASIO
    target_link_libraries(${PROJECT_NAME} PRIVATE 
        ws2_32 
        wsock32
    )
endif()

# For macOS, ensure proper threading and system libraries
if(APPLE)
    # Enable pthreads for threading support
    find_package(Threads REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
    
    # macOS-specific compiler flags for Boost ASIO
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        _DARWIN_C_SOURCE  # Enable POSIX extensions on macOS
    )
endif()
