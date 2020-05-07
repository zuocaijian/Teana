//
// Created by M70D5D0W on 2020/5/7.
//

#include "com_zcj_teana_opengles_VideoCore.h"
#include "../log_util.h"
#include "video_engine.h"

#ifdef __cplusplus
extern "C"
{
#endif

JNIEXPORT void JNICALL Java_com_zcj_teana_opengles_VideoCore_init
        (JNIEnv *env, jobject jClass)
{
    LOGI("call videoCore init");
}

/*
 * Class:     com_zcj_teana_opengles_VideoCore
 * Method:    play
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_zcj_teana_opengles_VideoCore_play
        (JNIEnv *env, jobject jClass)
{
    LOGI("call videoCore play");
}

#ifdef __cplusplus
}
#endif