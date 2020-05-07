//
// Created by M70D5D0W on 2020/5/7.
//

#ifndef TEANA_VIDEO_ENGINE_H
#define TEANA_VIDEO_ENGINE_H

#include <egl/egl.h>
#include <GLES2/gl2.h>

void initGLRender(int width, int height, GLuint *texts);

void playVideo(JNIEnv *env, const char *url, jobject surface);

#endif //TEANA_VIDEO_ENGINE_H
