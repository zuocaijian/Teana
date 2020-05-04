cmake_minimum_required(VERSION 3.4.1)

add_library(
        playcore
        SHARED
        src/main/jni/playcore.cpp
)

find_library(
        log-lib
        log
)

target_link_libraries(
        playcore
        ${log-lib}
)