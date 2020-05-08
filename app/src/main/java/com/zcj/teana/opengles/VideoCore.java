package com.zcj.teana.opengles;

import android.view.Surface;

public class VideoCore {
    static {
        System.loadLibrary("avutil");
        System.loadLibrary("swresample");
        System.loadLibrary("avcodec");
        System.loadLibrary("avformat");
        System.loadLibrary("swscale");
        System.loadLibrary("postproc");
        System.loadLibrary("avfilter");
        System.loadLibrary("avdevice");
        System.loadLibrary("videocore");
    }

    public native void init();

    public native void play(String url, Surface surface);
}
