//
// Created by M70D5D0W on 2020/5/7.
//

#include <android/window.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include "video_engine.h"
#include "../log_util.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include "libavutil/avutil.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"

#ifdef __cplusplus
}
#endif

void initEGL(JNIEnv *env, jobject surface)
{
    //1. EGLDisplay，对系统物理屏幕的封装
    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (display == EGL_NO_DISPLAY)
    {
        LOGE("eglGetDisplay failed!");
        return;
    }
    if (EGL_TRUE != eglInitialize(display, 0, 0))
    {
        LOGE("eglInitialize failed!");
        return;
    }

    //2. EGLConfig，在桥接OpenGL ES的输出和屏幕设备(EGLDisplay)的时候，指定一些配置项
    EGLConfig config;
    EGLint configNum;
    EGLint configSpec[] = {
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_NONE
    };
    if (EGL_TRUE != eglChooseConfig(display, configSpec, &config, 1, &configNum))
    {
        LOGE("eglChooseConfig failed!");
        return;
    }

    //3. EGLSurface，真正桥接OpenGL ES和屏幕设备
    //3.1 获取原始窗口，surface是外部的SurfaceView传递进来的
    ANativeWindow *win = ANativeWindow_fromSurface(env, surface);
    //3.2 创建EGLSurface
    EGLSurface winSurface = eglCreateWindowSurface(display, config, win, 0);
    if (winSurface == EGL_NO_SURFACE)
    {
        LOGE("eglCreateWindowSurface failed!");
        return;
    }

    //4. EGLContext，操作OpenGL的上下文，对OpenGL ES的操作都需要依赖这个上下文来实现
    const EGLint ctxAttr[] = {
            EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE
    };
    EGLContext context = eglCreateContext(display, config, EGL_NO_CONTEXT, ctxAttr);
    if (context == EGL_NO_CONTEXT)
    {
        LOGE("eglCreateContext failed!");
        return;
    }
    if (EGL_TRUE != eglMakeCurrent(display, winSurface, winSurface, context))
    {
        LOGE("eglMakeCurrent failed!");
        return;
    }
}

GLuint initShader(const char *code, GLenum type)
{
    //创建shader
    GLuint sh = glCreateShader(type);
    if (sh == GL_FALSE)
    {
        LOGE("glCreateShader %d failed!", type);
        return 0;
    }
    //加载shader
    glShaderSource(sh,
                   1, //shader数量
                   &code, //Shader代码
                   0); //代码长度
    // 编译Shader
    glCompileShader(sh);

    //获取编译情况
    GLint status;
    glGetShaderiv(sh, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        LOGE("glCompileShader failed!");
        return 0;
    }
    LOGI("glCompileShader success!");
    return sh;
}

GLint initProgram(GLuint &program)
{
    //1. 顶点和偏远着色器初始化
    //TODO 读取shader程序
    GLuint vsh = initShader(nullptr, GL_VERTEX_SHADER);
    GLuint fsh = initShader(nullptr, GL_FRAGMENT_SHADER);
    //2. 创建渲染程序
    //GLuint program = glCreateProgram();
    program = glCreateProgram();
    if (program == GL_FALSE)
    {
        LOGE("glCreateProgram failed!");
        return 0;
    }
    //3. 将着色器添加到渲染程序
    glAttachShader(program, vsh);
    glAttachShader(program, fsh);
    //4. 链接程序
    glLinkProgram(program);
    GLint status = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        LOGE("glLinkProgram failed~");
        return 0;
    }
    glUseProgram(program);
    LOGE("glLinkProgram success!");
}

void initGLRender(int width, int height, GLint (&texts)[3])
{
    GLuint program = 0;
    initProgram(program);

    //加入三维顶点数据 两个三角形组成正方形
    static float vers[] = {
            1.0f, -1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f
    };
    GLuint apos = (GLuint) glGetAttribLocation(program, "aTexCoord");
    glEnableVertexAttribArray(apos);
    //传递顶点
    glVertexAttribPointer(apos, 3, GL_FLOAT, GL_FALSE, 12, vers);

    //加入材质坐标数据
    static float txts[] = {
            1.0f, 0.0f, //右下
            0.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f
    };
    GLuint atex = (GLuint) glGetAttribLocation(program, "aTexCoord");
    glEnableVertexAttribArray(atex);
    glVertexAttribPointer(atex, 2, GL_FLOAT, GL_FALSE, 8, txts);

    //材质纹理初始化
    //设置纹理层
    glUniform1i(glGetUniformLocation(program, "yTexture"), 0); //纹理第1层
    glUniform1i(glGetUniformLocation(program, "uTexture"), 1); //纹理第2层
    glUniform1i(glGetUniformLocation(program, "vTexture"), 2); //纹理第3层

    //创建Open GL纹理
    //GLuint texts[3] = {0};
    //创建三个纹理
    glGenTextures(3, texts);
    //设置纹理属性
    glBindTexture(GL_TEXTURE_2D, texts[0]);
    //缩小的过滤器
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //设置纹理的格式和大小
    glTexImage2D(GL_TEXTURE_2D,
                 0,//细节 基本 0默认
                 GL_LUMINANCE, //gpu内部格式 亮度，灰度图
                 width, height, //拉升到全屏
                 0, //边框
                 GL_LUMINANCE, //像素的数据格式 亮度，灰度图 要与上面一致
                 GL_UNSIGNED_BYTE, //像素的数据类型
                 NULL //纹理的数据
    );

    //设置纹理属性
    glBindTexture(GL_TEXTURE_2D, texts[1]);
    //缩小的过滤器
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //设置纹理的格式和大小
    glTexImage2D(GL_TEXTURE_2D,
                 0, //细节 基本 0默认
                 GL_LUMINANCE, //gpu内部格式 亮度，灰度图
                 width / 2, height / 2, //拉升到全屏
                 0, //边框
                 GL_LUMINANCE,  //像素的数据格式 亮度，灰度图 要与上面一致
                 GL_UNSIGNED_BYTE, //像素的数据类型
                 NULL //纹理的数据
    );

    //设置纹理属性
    glBindTexture(GL_TEXTURE_2D, texts[2]);
    //缩小的过滤器
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //设置纹理的格式和大小
    glTexImage2D(GL_TEXTURE_2D,
                 0, //细节 基本 0默认
                 GL_LUMINANCE, //gpu内部格式 亮度，灰度图
                 width / 2, height / 2, //拉升到全屏
                 0, //边框
                 GL_LUMINANCE,  //像素的数据格式 亮度，灰度图 要与上面一致
                 GL_UNSIGNED_BYTE, //像素的数据类型
                 NULL //纹理的数据
    );
}

void renderVideo(int width, int height, AVFrame *frame)
{
    //纹理的修改和显示
    unsigned char *buf[3] = {0};
    buf[0] = new unsigned char[width * height];
    buf[1] = new unsigned char[width * height / 4];
    buf[2] = new unsigned char[width * height / 4];

    //数据Y
    //buf[0] = frame->data[0];
    memcpy(buf[0], frame->data[0], width * height);
    //数据U
    memcpy(buf[1], frame->data[1], width * height / 4);
    //数据V
    memcpy(buf[2], frame->data[2], width * height / 4);

    //激活第1层纹理，绑定到创建的OpenGL纹理
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, );
    //替换纹理内容
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_LUMINANCE, GL_UNSIGNED_BYTE, buf[0]);


}

void playVideo(JNIEnv *env, const char *videoPath, jobject surface)
{
    AVFormatContext *fmtCtx;
    fmtCtx = avformat_alloc_context();
    int re = avformat_open_input(&fmtCtx, videoPath, nullptr, nullptr);
    if (re != 0)
    {
        LOGE("打开视频文件失败：%s", av_err2str(re));
        return;
    }

    //探测流索引
    re = avformat_find_stream_info(fmtCtx, nullptr);
    if (re < 0)
    {
        LOGE("索引探测失败：%s", av_err2str(re));
        return;
    }

    //寻找视频流索引
    int vIdx = av_find_best_stream(fmtCtx, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
    if (vIdx == -1)
    {
        LOGE("获取视频流索引失败");
        return;
    }
    //解码器参数
    AVCodecParameters *codecParam;
    //解码器上下文
    AVCodecContext *codecCtx;
    //声明一个解码器
    const AVCodec *codec;

    codecParam = fmtCtx->streams[vIdx]->codecpar;
    //通过id查找解码器
    codec = avcodec_find_decoder(codecParam->codec_id);
    if (!codec)
    {
        LOGE("查找解码器失败");
        return;
    }

    //用解码器参数实例化编解码器上下文，并打开编解码器
    codecCtx = avcodec_alloc_context3(codec);
    //关联解码器上下文
    re = avcodec_parameters_to_context(codecCtx, codecParam);
    if (re < 0)
    {
        LOGE("解码器上下文关联失败：%s", av_err2str(re));
        return;
    }

    //打开解码器
    re = avcodec_open2(codecCtx, codec, nullptr);
    if (re < 0)
    {
        LOGE("打开解码器失败：%s", av_err2str(re));
        return;
    }

    //获取视频的宽高，也可以通过解码器获取
    int width = codecParam->width;
    int height = codecParam->height;
    LOGI("视频的宽 = %d, 高 = %d", width, height);

    //定义视频流、解封装后的数据包、解码后的数据帧
    AVStream *vStream = fmtCtx->streams[vIdx];
    AVPacket *pkt;
    AVFrame *frame;
    pkt = av_packet_alloc();
    frame = av_frame_alloc();

    initEGL(env, surface);
    GLuint texts[3] = {0};
    initGLRender(width, height, texts);

    while (av_read_frame(fmtCtx, pkt) >= 0)
    {
        //只节码视频流
        if (pkt->stream_index == vIdx)
        {
            //发送数据包到解码器
            re = avcodec_send_packet(codecCtx, pkt);
            //清理
            av_packet_unref(pkt);
            //一个AVPacket可能包含多个AVFrame，所以需要循环读取
            while (re >= 0)
            {
                re = avcodec_receive_frame(codecCtx, frame);
                if (re == AVERROR(EAGAIN) || re == AVERROR_EOF)
                {
                    return;
                }
                else if (re < 0)
                {
                    LOGE("解码失败：%s", av_err2str(re));
                    return;
                }

                //解码得到YUV数据，交给OpenGL ES显示
                renderVideo(width, height, frame);
            }
        }
    }

    //关闭环境
    avcodec_free_context(&codecCtx);
    //释放资源
    av_frame_free(&frame);
    av_packet_free(&pkt);
    avformat_free_context(fmtCtx);

    LOGI("播放完毕.");
}