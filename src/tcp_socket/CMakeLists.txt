# Include private header files
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Collect header and source files into variables
set(TCP_SOCKET_HEADERS "${TCP_PUBLIC_HEADERS}")
set(TCP_SOCKET_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/socket.cpp")

# Create static or shared libraries with the listed sources collected in the variables
if (BUILD_SHARED_LIBS)
        add_library(netsock_tcp_socket SHARED ${TCP_SOCKET_SOURCES} ${TCP_SOCKET_HEADERS})
else()
        add_library(netsock_tcp_socket STATIC ${TCP_SOCKET_SOURCES} ${TCP_SOCKET_HEADERS})
endif()

# Set up pre-compiled headers
target_precompile_headers(netsock_tcp_socket PRIVATE ${TCP_PRECOMPILED_HEADERS})

# Link the libraries
target_link_libraries(netsock_tcp_socket ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY})
target_link_libraries(netsock_tcp_socket spdlog::spdlog)

# Set default installation folders
install(TARGETS netsock_tcp_socket 
        RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
        LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib")