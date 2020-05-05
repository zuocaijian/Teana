cmake_minimum_required(VERSION 3.4.1)

# ------------- FFmpeg -------------

# 添加libavcodec.so
add_library(
        avcodec
        SHARED
        IMPORTED
)
set_target_properties(
        avcodec
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${CMAKE_ANDROID_ARCH_ABI}/libavcodec.so
)

# 添加libavdevice.so
add_library(
        avdevice
        SHARED
        IMPORTED
)
set_target_properties(
        avdevice
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${CMAKE_ANDROID_ARCH_ABI}/libavdevice.so
)

# 添加libavfilter.so
add_library(
        avfilter
        SHARED
        IMPORTED
)
SET_TARGET_PROPERTIES(
        avfilter
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${CMAKE_ANDROID_ARCH_ABI}/libavfilter.so
)

# 添加libavformat.so
add_library(
        avformat
        SHARED
        IMPORTED
)
SET_TARGET_PROPERTIES(
        avformat
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${CMAKE_ANDROID_ARCH_ABI}/libavformat.so
)

# 添加libavutil.so
add_library(
        avutil
        SHARED
        IMPORTED
)
SET_TARGET_PROPERTIES(
        avutil
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${CMAKE_ANDROID_ARCH_ABI}/libavutil.so
)

# 添加libpostproc.so
add_library(
        postproc
        SHARED
        IMPORTED
)
SET_TARGET_PROPERTIES(
        postproc
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${CMAKE_ANDROID_ARCH_ABI}/libpostproc.so
)

# 添加libswresample.so
add_library(
        swresample
        SHARED
        IMPORTED
)
SET_TARGET_PROPERTIES(
        swresample
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${CMAKE_ANDROID_ARCH_ABI}/libswresample.so
)

# 添加libswscale.so
add_library(
        swscale
        SHARED
        IMPORTED
)
SET_TARGET_PROPERTIES(
        swscale
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${CMAKE_ANDROID_ARCH_ABI}/libswscale.so
)

include_directories(src/main/jni/include)

add_library(
        ffmpegcore
        SHARED
        src/main/jni/ffmpeg_core.cpp
)

target_link_libraries(
        ffmpegcore
        avcodec
        avformat
        avutil
        swresample
        avdevice
        avfilter
        postproc
        swscale
)

# ------------- OpenSL ES -------------

add_library(
        playcore
        SHARED
        src/main/jni/play_core.cpp
        src/main/jni/play_engin.cpp
        src/main/jni/ffmpeg_engine.cpp
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
        avcodec
        avformat
        avutil
        swresample
        avdevice
        avfilter
        postproc
        swscale
)