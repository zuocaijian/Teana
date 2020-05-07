//
// Created by M70D5D0W on 2020/5/7.
//

#ifndef TEANA_VIDEO_ENGINE_H
#define TEANA_VIDEO_ENGINE_H

#include <egl/egl.h>
#include <GLES2/gl2.h>

void initEGL(JNIEnv *env, jobject surface);
void initGLRender(int width, int height, GLint (&texts)[3]);

#endif //TEANA_VIDEO_ENGINE_H
