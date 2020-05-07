package com.zcj.teana.opengles;

import android.view.Surface;

public class VideoCore {
    static {
        System.loadLibrary("videocore");
    }

    public native void init();

    public native void play(String url, Surface surface);
}
