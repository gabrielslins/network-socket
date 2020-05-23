# Set executable name
set(EXEC_NAME example_tcp_client)

# Include module header files
include_directories(${TCP_SOCKET}/include)
include_directories(${TCP_CLIENT}/include)

# Create executable
add_executable(${EXEC_NAME} main.cpp)

# Link the libraries
target_link_libraries(${EXEC_NAME} optimized netsock_tcp_client)
target_link_libraries(${EXEC_NAME} debug netsock_tcp_client)