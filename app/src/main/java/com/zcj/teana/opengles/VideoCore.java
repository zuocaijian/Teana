package com.zcj.teana.opengles;

public class VideoCore {
    static {
        System.loadLibrary("videocore");
    }

    public native void init();

    public native void play();
}
