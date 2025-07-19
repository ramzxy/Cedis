cmake_minimum_required(VERSION 3.20)
project(Cedis VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set CMake policies
if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

# Boost configuration - support multiple installation methods
if(DEFINED BOOST_ROOT)
    message(STATUS "Using BOOST_ROOT: ${BOOST_ROOT}")
    list(APPEND CMAKE_PREFIX_PATH ${BOOST_ROOT})
endif()

# Try to find Boost libraries (only system and thread are needed for Boost.Asio)
find_package(Boost REQUIRED COMPONENTS system thread)

# Add executable
add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/connection.cpp
    src/parser.cpp
    src/handleCMD.cpp
    src/database.cpp
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE 
    include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link Boost libraries (only system and thread needed for Boost.Asio)
target_link_libraries(${PROJECT_NAME} PRIVATE
    Boost::system
    Boost::thread
)

# Platform-specific settings
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

if(APPLE)
    # Enable pthreads for threading support
    find_package(Threads REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
    
    # macOS-specific compiler flags for Boost ASIO
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        _DARWIN_C_SOURCE  # Enable POSIX extensions on macOS
    )
endif()

# Debug mode settings
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG_MODE)
endif()

# Installation
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Print configuration summary
message(STATUS "")
message(STATUS "=== Cedis Configuration Summary ===")
message(STATUS "Project: ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Boost Version: ${Boost_VERSION}")
message(STATUS "Boost Root: ${Boost_ROOT}")
message(STATUS "Platform: ${CMAKE_SYSTEM_NAME}")
message(STATUS "=====================================")
message(STATUS "")
