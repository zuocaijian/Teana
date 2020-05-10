//
// Created by zcj on 2020/5/5.
//

#include <pthread.h>
#include "com_zcj_teana_ffmpeg_FFmpegCore.h"
#include "log_util.h"
#include "constant.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include "include/libavformat/avformat.h"
#include "include/libavcodec/avcodec.h"

void *test(void *arg)
{
    LOGI("参数 = %s", (char *) arg);
    return (void *) 0;
}

JNIEXPORT jint JNICALL Java_com_zcj_teana_ffmpeg_FFmpegCore_getFFmpegVersion
        (JNIEnv *env, jobject jObj)
{
    pthread_t pt;
    pthread_create(&pt, NULL, test, (void *) "123");
    int ret;
    pthread_join(pt, (void **) &ret);
    LOGI("原生线程返回值 = %d", ret);

    return avcodec_version();
}


JNIEXPORT jstring JNICALL Java_com_zcj_teana_ffmpeg_FFmpegCore_getFFmpegConfiguration
        (JNIEnv *env, jobject jObj)
{
    const char *config = avcodec_configuration();
    return env->NewStringUTF(config);
}

JNIEXPORT jstring JNICALL Java_com_zcj_teana_ffmpeg_FFmpegCore_getFFmpegLicense
        (JNIEnv *env, jobject jObj)
{
    // 获取所有协议
    LOGI("=== input protocols start===");
    void *opaque = NULL;
    const char *protocol = avio_enum_protocols(&opaque, 0);
    while (protocol != NULL)
    {
        LOGI("%s", protocol);
        protocol = avio_enum_protocols(&opaque, 0);
    }
    LOGI("=== input protocols end===");

    const char *license = avcodec_license();
    return env->NewStringUTF(license);
}

#ifdef __cplusplus
}
#endif