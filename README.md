# 项目名：天籁

> 天籁是一款基于`FFmpeg`和`OpenSL ES`的Android平台音乐播放器，支持播放本地文件和在线流媒体音乐

**项目准备分为以下几个步骤逐步推进：**
## OpenSL ES播放PCM音频文件
## FFmpeg的交叉编译及简单使用
## 串联FFmpeg和OpenSL ES，播放本地.mp3文件和在线流媒体http://xx.mp3文件
## 音乐播放器的接口抽象(状态回调、控制命令、音效处理等)
## 原生层的多线程等代码优化
## FFmpeg编译优化、裁剪
## 音效处理(fmod开源库???)
## 其他


1. [ffmpeg处理pcm和mp3互转](https://blog.csdn.net/weixin_33890499/article/details/88708622)
 1.1 mp3转pcm
    ```shell script
   ffmpeg -y -i test.mp3 -acodec pcm_s16le -f s16le -ac 2 -ar 44100 44k.pcm
   ```
   |参数|说明|
   |:--:|:--|
   |-y|允许覆盖|
   |-i test.mp3|源文件|
   |-acodec pcm_s16le|解码器|
   |-f s16le|强制文件格式|
   |-ac 2|双声道|
   |-ar 44100|采样率|
 1.2 pcm转mp3
    ```shell script
   ffmpeg -y -f s16le -ac 2 -ar 44100 -acodec pcm_s16le -i 44k.pcm new_test.mp3
    ```
   |参数|说明|
   |:--:|:--:|
   |-y|允许覆盖|
   |-f s16le|强制文件格式|
   |-ac 2|双声道|
   |-ar 44100|采样率|
   |-acodec pcm_s16le|编码器|
   |-i 44k.pcm|源文件|
2. OpenSL ES播放PCM
 2.1 播放流程
    创建并设置SL引擎 -> 创建并设置混音器 -> 创建并设置播放器 <- 设置回调并讲数据压入队列
 2.2 播放队列
    设置播放状态后，只要往播放队列压入数据，就会播放。从队列取出一个buf播放完毕后就会清理buf，队列非满，就会回调，压进pcm数据
 2.3 倍速播放
    如果原始音频采样率是44100，即一秒钟采样了44100次，播放的时候也是1秒钟播放44100个sample，那就是1.0的正常速度；如果播放的时候1秒钟播放44100 * 2个sample，那么就是2.0倍速了。
    可以通过设置音频格式配置SLDataFormat_PCM的采样率来控制1秒钟播放的sample数。
    不过这个方式设置倍速，会改变音频，听起来不舒服。推荐使用soundtouch库。
3. JNI调用FFmpeg
 3.1 FFmpeg的源码全部用的是C语言。如果在我们的项目中使用了C++，在.cpp文件中，**需要注意使用extern "C"" 把FFmpeg的头文件包裹起来**
4. Android NDK
 4.1 Android上不存在单独的libpthread或librt库，集成在libc中，无需显示链接。https://developer.android.google.cn/ndk/guides/stable_apis#c_library