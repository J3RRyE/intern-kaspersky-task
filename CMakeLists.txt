cmake_minimum_required(VERSION 3.10)
project(KasperskyInternTask)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

include_directories(${PROJECT_SOURCE_DIR}/common)

set(COMMON_SRC
    common/SocketUtils.cpp
    common/Utils.cpp
)

add_executable(client
    client/main.cpp
    client/Client.cpp
    client/Client.hpp
    ${COMMON_SRC}
)

add_executable(processing_server
    processing_server/main.cpp
    processing_server/ProcessingServer.cpp
    processing_server/ProcessingServer.hpp
    ${COMMON_SRC}
)

add_executable(display_server
    display_server/main.cpp
    display_server/DisplayServer.cpp
    display_server/DisplayServer.hpp
    ${COMMON_SRC}
)
