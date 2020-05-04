//
// Created by zcj on 2020/5/5.
//

#include "com_zcj_teana_ffmpeg_FFmpegCore.h"
#include "log_util.h"
#include "constant.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include "include/libavcodec/avcodec.h"

JNIEXPORT jint JNICALL Java_com_zcj_teana_ffmpeg_FFmpegCore_getFFmpegVersion
        (JNIEnv *env, jobject jClass)
{
    return avcodec_version();
}


JNIEXPORT jstring JNICALL Java_com_zcj_teana_ffmpeg_FFmpegCore_getFFmpegConfiguration
        (JNIEnv *env, jobject jClass)
{
    const char *config = avcodec_configuration();
    return env->NewStringUTF(config);
}

JNIEXPORT jstring JNICALL Java_com_zcj_teana_ffmpeg_FFmpegCore_getFFmpegLicense
        (JNIEnv *env, jobject jClass)
{
    const char *license = avcodec_license();
    return env->NewStringUTF(license);
}

#ifdef __cplusplus
}
#endif