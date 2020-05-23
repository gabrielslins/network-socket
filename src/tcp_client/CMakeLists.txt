# Create path variables
set(TCP_CLIENT_MINOR include/network_socket/tcp)

# Include module header files
include_directories(${TCP_SOCKET}/include)
include_directories(${TCP_CLIENT}/include)

# Collect header and source files into variables
set(TCP_CLIENT_HEADERS "${TCP_CLIENT}/${TCP_CLIENT_MINOR}/client.hpp")
set(TCP_CLIENT_SOURCES "${TCP_CLIENT}/src/client.cpp")

# Create static and shared libraries with the listed sources collected in the variables
add_library(netsock_tcp_client SHARED ${TCP_CLIENT_SOURCES} ${TCP_CLIENT_HEADERS})

# Link the libraries
target_link_libraries(netsock_tcp_client ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY})
target_link_libraries(netsock_tcp_client optimized netsock_tcp_socket)
target_link_libraries(netsock_tcp_client debug netsock_tcp_socket)

# Set default installation folders
install(TARGETS netsock_tcp_client 
        RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
        LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib")

install(DIRECTORY ${TCP_CLIENT}/include/ 
        DESTINATION "${CMAKE_INSTALL_PREFIX}/include")