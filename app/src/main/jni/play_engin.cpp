//
// Created by zcj on 2020/5/4.
//

//OpenSL ES 播放PCM音频文件
//播放步骤
//1. 创建并设置SL引擎
//2. 创建设置混音器(多路声音混合，音频输出设备)
//3. 创建设置播放器(控制播放)
//4. 设置回调并写入缓冲队列(存储播放数据)

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <cstdio>
#include "log_util.h"
#include "play_engine.h"

static SLObjectItf engineSL = NULL;
static const char *filePath = NULL;

#ifdef __cplusplus
extern "C"
{
#endif

//1. 创建并设置SL引擎
SLEngineItf createSL()
{
    //1. 创建引擎对象
    SLresult re;
    SLEngineItf en; //存引擎接口
    re = slCreateEngine(&engineSL, 0, 0, 0, 0, 0);
    if (re != SL_RESULT_SUCCESS)
    {
        return NULL;
    }

    //2. 实例化，阻塞等待实例化完成
    re = (*engineSL)->Realize(engineSL, SL_BOOLEAN_FALSE);
    if (re != SL_RESULT_SUCCESS)
    {
        return NULL;
    }

    //3. 获取接口
    re = (*engineSL)->GetInterface(engineSL, SL_IID_ENGINE, &en);
    if (re != SL_RESULT_SUCCESS)
    {
        return NULL;
    }

    return en;
}

void pcmCallback(SLAndroidSimpleBufferQueueItf caller, void *pContext)
{
    LOGI("pcmCallback...");
    static FILE *fp = NULL;
    static char *buf = NULL;
    if (!buf)
    {
        buf = new char[1024 * 1024];
    }
    if (!fp)
    {
        fp = fopen(filePath, "rb");
    }
    if (!fp)
    {
        LOGE("open pcm file failed.");
        return;
    }
    if (feof(fp) == 0)
    {
        int len = fread(buf, 1, 1024, fp);
        if (len > 0) //读取到数据，传入队列
        {
            (*caller)->Enqueue(caller, buf, len);
        }
    }
}

void playInternal(const char *pcmFile)
{
    filePath = pcmFile;

    //2. 创建设置混音器(多路声音混合，音频输出设备)

    SLEngineItf eng = createSL();
    SLresult re = 0;

    //2.1 输出混音器
    SLObjectItf mix = NULL;
    re = (*eng)->CreateOutputMix(eng, &mix, 0, 0, 0); //后面的配置项，可以做混音特效
    if (re != SL_RESULT_SUCCESS)
    {
        LOGE("createOutputMix failed.");
        return;
    }

    //2.2 实例化
    re = (*mix)->Realize(mix, SL_BOOLEAN_FALSE);
    if (re != SL_RESULT_SUCCESS)
    {
        LOGE("mix realize failed.");
        return;
    }

    //2.3 创建一个输出接口，给播放器调用
    SLDataLocator_OutputMix outmix = {SL_DATALOCATOR_OUTPUTMIX, mix};
    SLDataSink audioSink = {&outmix, 0};


    //3. 创建设置播放器(控制播放)
    //3.1 配置音频信息
    //缓冲队列
    SLDataLocator_AndroidBufferQueue queue = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 10};
    //音频格式配置
    SLDataFormat_PCM pcm = {
            SL_DATAFORMAT_PCM,
            2, //通道数
            SL_SAMPLINGRATE_44_1, //采样率
            SL_PCMSAMPLEFORMAT_FIXED_16, //bitsPerSample
            SL_PCMSAMPLEFORMAT_FIXED_16, //containerSize
            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT, //声道
            SL_BYTEORDER_LITTLEENDIAN //字节序，小端模式
    };
    //播放器使用的结构体
    SLDataSource ds = {&queue, &pcm};

    //3.2 播放器对象
    SLObjectItf player = NULL;
    SLPlayItf playerInterface = NULL;
    SLAndroidSimpleBufferQueueItf pcmQueue = NULL;
    const SLInterfaceID ids[] = {SL_IID_BUFFERQUEUE}; //接口id
    const SLboolean req[] = {SL_BOOLEAN_TRUE}; //接口开放
    re = (*eng)->CreateAudioPlayer(eng, &player, &ds, &audioSink,
                                   sizeof(ids) / sizeof(SLInterfaceID), ids, req);
    if (re != SL_RESULT_SUCCESS)
    {
        LOGE("createAudioPlayer failed.");
    }
    else
    {
        LOGI("createAudioPlayer success.");
    }

    //实例化
    (*player)->Realize(player, SL_BOOLEAN_FALSE);
    //获取接口
    re = (*player)->GetInterface(player, SL_IID_PLAY, &playerInterface);
    if (re != SL_RESULT_SUCCESS)
    {
        LOGE("player getInterface failed.");
    }
    else
    {
        LOGI("player getInterface success.");
    }

    //获取缓冲队列接口
    re = (*player)->GetInterface(player, SL_IID_BUFFERQUEUE, &pcmQueue);
    if (re != SL_RESULT_SUCCESS)
    {
        LOGE("player getInterface bufferQueue failed.");
    }
    else
    {
        LOGI("player getInterface bufferQueue success.");
    }

    //设置回调函数，播放队列为空的时候调用
    //第二个参数是回调函数，第三个参数是给回调函数传的参数
    (*pcmQueue)->RegisterCallback(pcmQueue, pcmCallback, 0);
    //设置状态 播放
    (*playerInterface)->SetPlayState(playerInterface, SL_PLAYSTATE_PLAYING);
    //启动队列回调
    (*pcmQueue)->Enqueue(pcmQueue, "", 1);
}

#ifdef __cplusplus
}
#endif