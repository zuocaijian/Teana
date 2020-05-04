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