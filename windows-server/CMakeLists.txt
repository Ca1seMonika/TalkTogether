cmake_minimum_required(VERSION 3.17)
project(out/TalkTogether-Server)

set(CMAKE_CXX_STANDARD 11)

FILE(GLOB SOURCES
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        "${PROJECT_SOURCE_DIR}/header/*.h"
        )

include_directories(${PROJECT_SOURCE_DIR}/Mysql)
include_directories(${PROJECT_SOURCE_DIR}/header)
add_executable(TalkTogether-Server ${SOURCES} main.cpp)
target_link_libraries(TalkTogether-Server ws2_32 mysql)