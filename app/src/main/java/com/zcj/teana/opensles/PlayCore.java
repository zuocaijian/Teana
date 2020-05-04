package com.zcj.teana.opensles;

public class PlayCore {
    static {
        System.loadLibrary("playcore");
    }

    public native int init();

    public native int play(String fileName);

    public native int pause();

    public native int stop();

    public native int destroy();
}
