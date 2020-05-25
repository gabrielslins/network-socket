# Include private header files
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Collect header and source files into variables
set(TCP_CLIENT_HEADERS "")
set(TCP_CLIENT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/client.cpp")

# Create static or shared libraries with the listed sources collected in the variables
if (BUILD_SHARED_LIBS)
        add_library(netsock_tcp_client SHARED ${TCP_CLIENT_SOURCES} ${TCP_CLIENT_HEADERS})
else()
        add_library(netsock_tcp_client STATIC ${TCP_CLIENT_SOURCES} ${TCP_CLIENT_HEADERS})
endif()

# Link the libraries
target_link_libraries(netsock_tcp_client ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY})
target_link_libraries(netsock_tcp_client optimized netsock_tcp_socket)
target_link_libraries(netsock_tcp_client debug netsock_tcp_socket)

# Set default installation folders
install(TARGETS netsock_tcp_client 
        RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
        LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib")