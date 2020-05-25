# Include private header files
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Collect header and source files into variables
set(TCP_SERVER_HEADERS "${TCP_PUBLIC_HEADERS}")
set(TCP_SERVER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/server.cpp")

# Create static or shared libraries with the listed sources collected in the variables
if (BUILD_SHARED_LIBS)
        add_library(netsock_tcp_server SHARED ${TCP_SERVER_SOURCES} ${TCP_SERVER_HEADERS})
else()
        add_library(netsock_tcp_server STATIC ${TCP_SERVER_SOURCES} ${TCP_SERVER_HEADERS})
endif()

# Link the libraries
target_link_libraries(netsock_tcp_server ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY})
target_link_libraries(netsock_tcp_server optimized netsock_tcp_socket)
target_link_libraries(netsock_tcp_server debug netsock_tcp_socket)

# Set default installation folders
install(TARGETS netsock_tcp_server 
        RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
        LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib")