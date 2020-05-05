//
// Created by zcj on 2020/5/4.
//
#include <android/log.h>

#ifndef TEANA_LOGUTIL_H
#define TEANA_LOGUTIL_H

#define TEANA_PCM_TAG "teana_pcm"

#define LOGD(FORMAT, ...) __android_log_print(ANDROID_LOG_DEBUG, TEANA_PCM_TAG, FORMAT, ##__VA_ARGS__)
#define LOGI(FORMAT, ...) __android_log_print(ANDROID_LOG_INFO, TEANA_PCM_TAG, FORMAT, ##__VA_ARGS__)
#define LOGW(FORMAT, ...) __android_log_print(ANDROID_LOG_WARN, TEANA_PCM_TAG, FORMAT, ##__VA_ARGS__)
#define LOGE(FORMAT, ...) __android_log_print(ANDROID_LOG_ERROR, TEANA_PCM_TAG, FORMAT, ##__VA_ARGS__)

#endif //TEANA_LOGUTIL_H
