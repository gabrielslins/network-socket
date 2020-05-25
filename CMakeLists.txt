# Minimum CMake version required
cmake_minimum_required(VERSION 3.16)

# Project name
project(network-socket)

# Project options
option(BUILD_EXAMPLES "Build examples" OFF)
option(BUILD_SHARED_LIBS "Build shared libs" ON)

# Turn on the ability to create folders to organize projects
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Set default static and shared libraries output path
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib" CACHE PATH "Default static libraries output path")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin" CACHE PATH "Default shared libraries output path")

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
find_package(Boost 1.72 REQUIRED COMPONENTS system thread regex)

if (${Boost_FOUND})
    # Add Boost include directories to those the compiler uses to search for include files
    include_directories(${Boost_INCLUDE_DIRS})

    # Add Boost library directories to the list of directories to use for the linking step
    link_directories(${Boost_LIBRARY_DIRS})
endif()

# Include public headers directory
include_directories(${PROJECT_SOURCE_DIR}/include)

# Set variables related to TCP public headers
set(TCP_PUBLIC_HEADERS_DIR "${PROJECT_SOURCE_DIR}/include/network_socket/tcp")
set(TCP_PUBLIC_HEADERS "${TCP_PUBLIC_HEADERS_DIR}/client.hpp"
                       "${TCP_PUBLIC_HEADERS_DIR}/server.hpp"
                       "${TCP_PUBLIC_HEADERS_DIR}/socket.hpp")

# Add project modules subdirectories
add_subdirectory(src/tcp_socket)
add_subdirectory(src/tcp_client)
add_subdirectory(src/tcp_server)

# Build examples if option is on
if(BUILD_EXAMPLES)
    # Status message
    message(STATUS "Examples will be built")

    # Add examples subdirectories
    add_subdirectory(examples/tcp_client)
    add_subdirectory(examples/tcp_server)
endif(BUILD_EXAMPLES)

# Check if shared or static libraries will be built
if(BUILD_SHARED_LIBS)
    # Status message
    message(STATUS "Shared libraries will be built")

    # Create 'bin' directory in root if it doesn't exist already
    add_custom_target(build-time-make-bin-directory ALL COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_SOURCE_DIR}/bin)
else()
    # Status message
    message(STATUS "Static libraries will be built")

    # Create 'lib' directory in root if it doesn't exist already
    add_custom_target(build-time-make-lib-directory ALL COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_SOURCE_DIR}/lib)
endif()