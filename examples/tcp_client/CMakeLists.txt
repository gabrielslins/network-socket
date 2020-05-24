# Set executable name
set(EXEC_NAME example_tcp_client)

# Create executable
add_executable(${EXEC_NAME} main.cpp)

# Link the libraries
target_link_libraries(${EXEC_NAME} optimized netsock_tcp_client)
target_link_libraries(${EXEC_NAME} debug netsock_tcp_client)