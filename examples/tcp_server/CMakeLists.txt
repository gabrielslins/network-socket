# Set executable name
set(EXEC_NAME example_tcp_server)

# Collect header and source files into variables
set(EXAMPLE_TCP_SERVER_HEADERS "")
set(EXAMPLE_TCP_SERVER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

# Create executable
add_executable(${EXEC_NAME} ${EXAMPLE_TCP_SERVER_SOURCES} ${EXAMPLE_TCP_SERVER_HEADERS})

# Link the libraries
target_link_libraries(${EXEC_NAME} optimized netsock_tcp_server)
target_link_libraries(${EXEC_NAME} debug netsock_tcp_server)