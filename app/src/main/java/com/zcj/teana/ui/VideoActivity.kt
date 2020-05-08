package com.zcj.teana.ui

import android.content.Context
import android.os.Bundle
import android.os.PowerManager
import android.util.Log
import android.view.Surface
import android.view.SurfaceHolder
import android.view.View
import androidx.appcompat.app.AppCompatActivity
import com.zcj.teana.App
import com.zcj.teana.R
import com.zcj.teana.opengles.VideoCore
import kotlinx.android.synthetic.main.activity_video.*

/**
 * 1. Android使用OpenGL渲染ffmpeg解码的YUV视频数据
 * https://blog.csdn.net/u012944685/article/details/104586591
 *
 * 2. 基于FFmpeg解码和OpenGL ES渲染的Android视频播放
 * https://blog.csdn.net/ericbar/article/details/80506390
 *
 * 2. MP4视频测试URL地址，亲测有效
 * https://blog.csdn.net/weixin_41010198/article/details/88055078?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-2&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-2
 */
class VideoActivity : AppCompatActivity() {

    private val videoCore = VideoCore()

    private var surface: Surface? = null;

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_video)

        sv.holder.addCallback(object : SurfaceHolder.Callback {
            override fun surfaceChanged(
                holder: SurfaceHolder?,
                format: Int,
                width: Int,
                height: Int
            ) {
            }

            override fun surfaceDestroyed(holder: SurfaceHolder?) {
            }

            override fun surfaceCreated(holder: SurfaceHolder?) {
                surface = holder!!.surface
            }
        })

    }

    fun init(view: View) {
        videoCore.init()
    }

    fun tmpFun(view: View) {
        val pm: PowerManager = getSystemService(Context.POWER_SERVICE) as PowerManager
        val wl: PowerManager.WakeLock =
            pm.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, "Teana:WakeLockTag")
        wl.acquire()

        val mp4Path =
            "${App.sContext.getExternalFilesDir(null)}${java.io.File.separator}big_buck_bunny.mp4"
        val mp4OceansPath =
            "${App.sContext.getExternalFilesDir(null)}${java.io.File.separator}oceans.mp4"
        val mp3Path =
            "${App.sContext.getExternalFilesDir(null)}${java.io.File.separator}test.mp3";
        videoCore.play(mp4Path, surface)
    }
}