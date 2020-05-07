package com.zcj.teana.ui

import android.os.Bundle
import android.view.View
import androidx.appcompat.app.AppCompatActivity
import com.zcj.teana.R
import com.zcj.teana.opengles.VideoCore

/**
 * Android使用OpenGL渲染ffmpeg解码的YUV视频数据
 * https://blog.csdn.net/u012944685/article/details/104586591
 */
class VideoActivity : AppCompatActivity() {

    private val videoCore = VideoCore()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_video)
    }

    fun init(view: View) {
        videoCore.init()
    }

    fun tmpFun(view: View) {
        videoCore.play()
    }
}