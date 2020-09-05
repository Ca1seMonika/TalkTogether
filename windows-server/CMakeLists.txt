cmake_minimum_required(VERSION 3.10)
project(TalkTogether-Server)

set(CMAKE_CXX_STANDARD 11)

if(CMAKE_HOST_WIN32)
FILE(GLOB SOURCES
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        "${PROJECT_SOURCE_DIR}/header/*.h"
        )

include_directories(${PROJECT_SOURCE_DIR}/Mysql)
include_directories(${PROJECT_SOURCE_DIR}/header)
add_executable(TalkTogether-Server ${SOURCES} main.cpp)


add_custom_command(TARGET TalkTogether-Server POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/lib/libmysql.dll ${PROJECT_SOURCE_DIR}/out/libmysql.dll
        COMMAND echo host= > ${PROJECT_SOURCE_DIR}/out/config.ini
        COMMAND echo user= >> ${PROJECT_SOURCE_DIR}/out/config.ini
        COMMAND echo password= >> ${PROJECT_SOURCE_DIR}/out/config.ini
        COMMAND echo port= >> ${PROJECT_SOURCE_DIR}/out/config.ini
)

target_link_libraries(TalkTogether-Server ws2_32 mysql)
else()
    message(STATUS "Only for windows")
endif()