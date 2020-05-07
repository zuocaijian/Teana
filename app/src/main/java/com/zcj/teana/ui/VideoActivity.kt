package com.zcj.teana.ui

import android.os.Bundle
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
 * 2. MP4视频测试URL地址，亲测有效
 * https://blog.csdn.net/weixin_41010198/article/details/88055078?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-2&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-2
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
        val mp4Path =
            "${App.sContext.getExternalFilesDir(null)}${java.io.File.separator}big_buck_bunny.mp4";
        videoCore.play(mp4Path, sv.holder.surface)
    }
}