# Create path variables
set(TCP_SOCKET_MINOR include/network_socket/tcp)

# Include module header files
include_directories(${TCP_SOCKET}/include)

# Collect header and source files into variables
set(TCP_SOCKET_HEADERS "${TCP_SOCKET}/${TCP_SOCKET_MINOR}/socket.hpp")
set(TCP_SOCKET_SOURCES "${TCP_SOCKET}/src/socket.cpp")

# Create static and shared libraries with the listed sources collected in the variables
add_library(netsock_tcp_socket SHARED ${TCP_SOCKET_SOURCES} ${TCP_SOCKET_HEADERS})

# Link the libraries
target_link_libraries(netsock_tcp_socket ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY})

# Set default installation folders
install(TARGETS netsock_tcp_socket 
        RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
        LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib")

install(DIRECTORY ${TCP_SOCKET}/include/ 
        DESTINATION "${CMAKE_INSTALL_PREFIX}/include")