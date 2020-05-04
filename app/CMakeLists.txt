cmake_minimum_required(VERSION 3.4.1)

add_library(
        playcore
        SHARED
        src/main/jni/playcore.cpp
        src/main/jni/play_engin.cpp
)

find_library(
        log-lib
        log
)

find_library(
        sles-lib
        OpenSLES
)

target_link_libraries(
        playcore
        ${log-lib}
        ${sles-lib}
)