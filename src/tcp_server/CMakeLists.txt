# Create path variables
set(TCP_SERVER_MINOR include/network_socket/tcp)

# Include module header files
include_directories(${TCP_SOCKET}/include)
include_directories(${TCP_SERVER}/include)

# Collect header and source files into variables
set(TCP_SERVER_HEADERS "${TCP_SERVER}/${TCP_SERVER_MINOR}/server.hpp")
set(TCP_SERVER_SOURCES "${TCP_SERVER}/src/server.cpp")

# Create static and shared libraries with the listed sources collected in the variables
add_library(netsock_tcp_server SHARED ${TCP_SERVER_SOURCES} ${TCP_SERVER_HEADERS})

# Link the libraries
target_link_libraries(netsock_tcp_server ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY})
target_link_libraries(netsock_tcp_server optimized netsock_tcp_socket)
target_link_libraries(netsock_tcp_server debug netsock_tcp_socket)

# Set default installation folders
install(TARGETS netsock_tcp_server 
        RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
        LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib")

install(DIRECTORY ${TCP_SERVER}/include/ 
        DESTINATION "${CMAKE_INSTALL_PREFIX}/include")