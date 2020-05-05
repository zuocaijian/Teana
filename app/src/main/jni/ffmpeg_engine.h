//
// Created by zcj on 2020/5/5.
//

#ifndef TEANA_FFMPEG_ENGINE_H
#define TEANA_FFMPEG_ENGINE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "include/libavformat/avformat.h"
#include "include/libavcodec/avcodec.h"
#include "include/libswresample/swresample.h"

int createFFmpeg(unsigned *rate, unsigned *channel);
int getPCM(void **pcm, size_t *pcm_size);
void releaseFFmpeg();

#ifdef __cplusplus
}
#endif

#endif //TEANA_FFMPEG_ENGINE_H
