//
// Created by zcj on 2020/5/4.
//

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "com_zcj_teana_pcm_control.h"
#include "log_util.h"
#include "constant.h"
#include "play_engine.h"

#ifdef __cplusplus
extern "C"
{
#endif

JNIEXPORT jint JNICALL Java_com_zcj_teana_pcm_Control_init
        (JNIEnv *env, jobject jClass)
{
    LOGD("init is called.");
    return SUCCESS;
}


JNIEXPORT jint JNICALL Java_com_zcj_teana_pcm_Control_play
        (JNIEnv *env, jobject jClass, jstring jStr)
{
    const char *suffix = "play is called, prepare to play ";
    jboolean isCopy;
    const char *filePath = env->GetStringUTFChars(jStr, &isCopy);
    auto len1 = strlen(suffix);
    auto len2 = strlen(filePath);
    char *msg = (char *) malloc(sizeof(char) * (len1 + len2 + 1));
    memset(msg, 0, (len1 + len2 + 1));
    memcpy(msg, suffix, len1);
    memcpy(msg + len1, filePath, len2);
    LOGD(msg);

    playInternal(filePath);
    return SUCCESS;
}

JNIEXPORT jint JNICALL Java_com_zcj_teana_pcm_Control_pause
        (JNIEnv *env, jobject jClass)
{
    LOGD("pause is called.");
    return SUCCESS;
}

JNIEXPORT jint JNICALL Java_com_zcj_teana_pcm_Control_stop
        (JNIEnv *env, jobject jClass)
{
    LOGD("stop is called.");
    return SUCCESS;
}

JNIEXPORT jint JNICALL Java_com_zcj_teana_pcm_Control_destroy
        (JNIEnv *env, jobject jClass)
{
    LOGD("destroy is called.");
    return SUCCESS;
}

#ifdef __cplusplus
}
#endif