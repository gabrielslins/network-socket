# Set executable name
set(EXEC_NAME example_tcp_client)

# Collect header and source files into variables
set(EXAMPLE_TCP_CLIENT_HEADERS "")
set(EXAMPLE_TCP_CLIENT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

# Create executable
add_executable(${EXEC_NAME} ${EXAMPLE_TCP_CLIENT_SOURCES} ${EXAMPLE_TCP_CLIENT_HEADERS})

# Link the libraries
target_link_libraries(${EXEC_NAME} optimized netsock_tcp_client)
target_link_libraries(${EXEC_NAME} debug netsock_tcp_client)