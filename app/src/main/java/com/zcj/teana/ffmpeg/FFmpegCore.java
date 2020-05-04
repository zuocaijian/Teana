package com.zcj.teana.ffmpeg;

public class FFmpegCore {
    static {
        System.loadLibrary("avutil");
        System.loadLibrary("swresample");
        System.loadLibrary("avcodec");
        System.loadLibrary("avformat");
        System.loadLibrary("swscale");
        System.loadLibrary("postproc");
        System.loadLibrary("avfilter");
        System.loadLibrary("avdevice");
        System.loadLibrary("ffmpegcore");
    }

    public native int getFFmpegVersion();

    public native String getFFmpegConfiguration();

    public native String getFFmpegLicense();
}
