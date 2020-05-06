//
// Created by zcj on 2020/5/4.
//

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <pthread.h>
#include "com_zcj_teana_opensles_playcore.h"
#include "log_util.h"
#include "constant.h"
#include "play_engine.h"

#ifdef __cplusplus
extern "C"
{
#endif

JavaVM *jvm;

JNIEXPORT jint JNICALL Java_com_zcj_teana_opensles_PlayCore_init
        (JNIEnv *env, jobject jClass)
{
    LOGD("init is called.");
    return SUCCESS;
}


JNIEXPORT jint JNICALL Java_com_zcj_teana_opensles_PlayCore_play
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

JNIEXPORT jint JNICALL Java_com_zcj_teana_opensles_PlayCore_pause
        (JNIEnv *env, jobject jClass)
{
    LOGD("pause is called.");
    return SUCCESS;
}

JNIEXPORT jint JNICALL Java_com_zcj_teana_opensles_PlayCore_stop
        (JNIEnv *env, jobject jClass)
{
    LOGD("stop is called.");
    return SUCCESS;
}

JNIEXPORT jint JNICALL Java_com_zcj_teana_opensles_PlayCore_destroy
        (JNIEnv *env, jobject jClass)
{
    LOGD("destroy is called.");
    return SUCCESS;
}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    jvm = vm;
    return JNI_VERSION_1_4;
}

#ifdef __cplusplus
}
#endif