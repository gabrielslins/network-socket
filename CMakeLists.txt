# Minimum CMake version required
cmake_minimum_required(VERSION 3.16)

# Project name
project(network-socket)

# Set default install path
set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/install" CACHE PATH "Default install path" FORCE)
# set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/install" CACHE PATH "Default install path")

# Turn on the ability to create folders to organize projects
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Add to path folder containing Find<package>.cmake files
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# Set C++11 as standard
set(CMAKE_CXX_STANDARD 11)

# Check which C++ compiler is going to be used and set its flags
if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    message("No Clang flags set")
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    # Status message
    message(STATUS "Setting g++ flags")

    # Set C++ compiler flags
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -j8")   # Build with multiple cores
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "Intel")
    message("No Intel C++ flags set")
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    # Status message
    message(STATUS "Setting MSVC flags")

    # Set C++ compiler flags
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")   # Build with multiple cores
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc") # Standard C++ exception handling
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "AppleClang")
    message("No Apple Clang flags set")
else()
    message("No C++ compiler flags set")
endif()

# Rename generated libraries in DEBUG case
if(NOT CMAKE_DEBUG_POSTFIX)
    set(CMAKE_DEBUG_POSTFIX "_d")
endif()

# Look for Boost required components
find_package(Boost 1.66 REQUIRED COMPONENTS system thread regex)

if (${Boost_FOUND})
    # Add Boost include directories to those the compiler uses to search for include files
    include_directories(${Boost_INCLUDE_DIRS})

    # Add Boost library directories to the list of directories to use for the linking step
    link_directories(${Boost_LIBRARY_DIRS})
endif()

# Create path variables
set(TCP_SOCKET ${PROJECT_SOURCE_DIR}/src/tcp_socket)
set(TCP_CLIENT ${PROJECT_SOURCE_DIR}/src/tcp_client)
set(TCP_SERVER ${PROJECT_SOURCE_DIR}/src/tcp_server)

# Add the respective subdirectories of the project
add_subdirectory(src/tcp_socket)
add_subdirectory(src/tcp_client)
add_subdirectory(src/tcp_server)
