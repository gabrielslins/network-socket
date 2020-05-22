# Create path variables
set(TCP_CLIENT ${PROJECT_SOURCE_DIR}/src/tcp_client)
set(TCP_CLIENT_MINOR include/network_socket/tcp_client)

# Include module header files
include_directories(${TCP_CLIENT}/include)

# Collect header and source files into variables
set(TCP_CLIENT_HEADERS "${TCP_CLIENT}/${TCP_CLIENT_MINOR}/tcp_client.hpp")
set(TCP_CLIENT_SOURCES "${TCP_CLIENT}/src/tcp_client.cpp")

# Create static and shared libraries with the listed sources collected in the variables
add_library(netsock_tcp_client SHARED ${TCP_CLIENT_SOURCES} ${TCP_CLIENT_HEADERS})

# Link the libraries
target_link_libraries(netsock_tcp_client ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY})

# Set default installation folders
install(TARGETS netsock_tcp_client 
        RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
        LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib")

install(DIRECTORY ${TCP_CLIENT}/include/ 
        DESTINATION "${CMAKE_INSTALL_PREFIX}/include")