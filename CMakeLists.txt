cmake_minimum_required(VERSION 3.2)

project(SocketCAN)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add the following line to enable debugging information
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# Find and link against the threading library
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

file(GLOB CPP_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

# Add headers
file(GLOB HEADERS 
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

add_executable(${PROJECT_NAME} ${CPP_SOURCES} ${HEADERS})

# Link against the threading library
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
