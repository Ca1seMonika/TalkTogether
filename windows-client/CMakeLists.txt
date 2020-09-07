cmake_minimum_required(VERSION 3.16)
project(TalkTogether-Client)

set(CMAKE_CXX_STANDARD 11)
if(CMAKE_HOST_WIN32)
    file(GLOB SOURCE_FILE
            ${PROJECT_SOURCE_DIR}/src/*.cpp
            ${PROJECT_SOURCE_DIR}/header/*.h
            )

    include_directories(${PROJECT_SOURCE_DIR}/header)
    add_executable(TalkTogether-Client ${SOURCE_FILE} main.cpp)
    target_link_libraries(TalkTogether-Client ws2_32)

    add_custom_command(TARGET TalkTogether-Client POST_BUILD
            COMMAND echo server_host=> ${PROJECT_SOURCE_DIR}/out/config.ini
            )
else()
    message("only for windows")
endif()